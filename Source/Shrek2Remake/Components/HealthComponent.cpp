// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	bAutoActivate = true;
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter->LandedDelegate.AddDynamic(this, &UHealthComponent::TryApplyFallDamage);
	}
}

// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth, bool AbjustHealthToMax)
{
	MaxHealth = NewMaxHealth;

	if (AbjustHealthToMax)
	{
		Health = MaxHealth;
	}

	OnMaxHealthChanged.Broadcast(this, MaxHealth);
	OnHealthChanged.Broadcast(this, Health);
}

void UHealthComponent::ApplyDamage(FDamageInfo DamageInfo)
{
	if (!IsActive())
		return;

	if (Health > 0 && bCanBeDamaged)
	{
		if (!bImmortal)
		{
			Health -= DamageInfo.Value;
			OnHealthChanged.Broadcast(this, Health);
		}

		OnDamaged.Broadcast(this, DamageInfo);

		if (Health <= 0)
		{
			HandleDeath(DamageInfo);
		}
	}
}

void UHealthComponent::AddHealth(float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0, MaxHealth);

	OnHealed.Broadcast(this, HealthToAdd);
	OnHealthChanged.Broadcast(this, Health);
}

void UHealthComponent::HandleDeath(FDamageInfo LastDamageInfo)
{
	OnDied.Broadcast(this, LastDamageInfo);
}

void UHealthComponent::Die(FDamageInfo LastDamageInfo)
{
	LastDamageInfo.Value = 999999;
	ApplyDamage(LastDamageInfo);
}

void UHealthComponent::TryApplyFallDamage(const FHitResult& LandedHitResult)
{
	if (bNoFallDamageGranted)
	{
		bNoFallDamageGranted = false;
		return;
	}

	if (!bCanBeDamaged || bImmortal || FallDamage <= 0 || FallDamageMinVelocity <= 0)
		return;

	if (LandedHitResult.PhysMaterial.IsValid())
	{
		EPhysicalSurface Surface = UPhysicalMaterial::DetermineSurfaceType(LandedHitResult.PhysMaterial.Get());

		if (NoFallDamageSurfaces.Contains(Surface))
			return;
	}

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (-OwnerCharacter->GetVelocity().Z > FallDamageMinVelocity)
		{
			FDamageInfo DamageInfo;
			DamageInfo.DamageHit = LandedHitResult;
			DamageInfo.Value = FallDamage;

			ApplyDamage(DamageInfo);
		}
	}
}

void UHealthComponent::GrantNoFallDamage()
{
	bNoFallDamageGranted = true;
}

