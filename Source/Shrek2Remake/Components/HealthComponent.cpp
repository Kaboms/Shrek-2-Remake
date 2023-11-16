// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Health = MaxHealth;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
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
	if (Health > 0)
	{
		Health -= DamageInfo.Value;

		OnDamaged.Broadcast(this, DamageInfo);
		OnHealthChanged.Broadcast(this, Health);

		if (Health <= 0)
		{
			Die(DamageInfo);
		}
	}
}

void UHealthComponent::AddHealth(float HealthToAdd)
{
	Health = FMath::Clamp(Health + HealthToAdd, 0, MaxHealth);

	OnHealed.Broadcast(this, HealthToAdd);
	OnHealthChanged.Broadcast(this, Health);
}

void UHealthComponent::Die(FDamageInfo LastDamageInfo)
{
	OnDied.Broadcast(this, LastDamageInfo);
}

