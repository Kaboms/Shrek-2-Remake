// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "Components/FightComponent.h"
#include "Core/GameplayTagsNative.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HealthComponent))
	{
		HealthComponent->OnDamaged.AddDynamic(this, &ACharacterBase::OnDamageReceived);
		HealthComponent->OnDied.AddDynamic(this, &ACharacterBase::OnDied);
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ACharacterBase::OnPlayMontageNotifyBegin);
	}
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UActorComponent* FindedFightComponent = GetComponentByClass(UFightComponent::StaticClass());
	if (IsValid(FindedFightComponent))
	{
		FightComponent = Cast<UFightComponent>(FindedFightComponent);
	}
}

bool ACharacterBase::GetIsAttack() const
{
	if (IsValid(FightComponent))
	{
		return FightComponent->bIsAttack;
	}
	return false;
}

void ACharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (IsValid(FightComponent))
	{
		FightComponent->InterruptAttack();
	}
}

void ACharacterBase::SetStuned(bool bInStuned)
{
	bStuned = bInStuned;
	ReceiveOnStuned(bStuned);
}

UAnimMontage* ACharacterBase::FindTagedMontage(TArray<FTagedAnimMontage>& Montages, FGameplayTagContainer MontageTags, UAnimMontage* DefaultMontage)
{
	if (!MontageTags.IsEmpty())
	{
		FTagedAnimMontage* TagedAnimMontage = Montages.FindByPredicate([&](FTagedAnimMontage TagedAnimMontage) {
			return TagedAnimMontage.MontageTags.HasAllExact(MontageTags);
			});

		if (TagedAnimMontage)
		{
			return TagedAnimMontage->Montage;
		}
	}
	return DefaultMontage;
}

void ACharacterBase::OnPlayMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (NotifyName == FName(TEXT("DeathEnded")))
	{
		OnDeathMontageEnded();
	}
}

void ACharacterBase::OnDamageReceived(UHealthComponent* InHealthComponent, FDamageInfo DamageInfo)
{
	if (IsValid(FightComponent))
	{
		FightComponent->InterruptAttack();
	}

	if (HealthComponent->IsAlive())
	{
		SetStuned(true);

		if (!DamageInfo.ImpactPoint.IsZero())
		{
			if (FVector::DotProduct(GetActorForwardVector(), UKismetMathLibrary::Normal(DamageInfo.ImpactPoint - GetActorLocation())) >= 0)
			{
				DamageInfo.DamageIdentifier.AddTag(FGameplayTagsNative::Get().Damage_Direction_Front);
			}
			else
			{
				DamageInfo.DamageIdentifier.AddTag(FGameplayTagsNative::Get().Damage_Direction_Back);
			}
		}

		UAnimMontage* MontageToPlay = FindTagedMontage(OnDamageMontages, DamageInfo.DamageIdentifier, DefaultDamageMontage);

		FOnMontageEnded OnMontageEndedDelegate;
		OnMontageEndedDelegate.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted)
			{
				if (HealthComponent->IsAlive())
					SetStuned(false);
			});

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(MontageToPlay);
			AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, MontageToPlay);
		}
	}

	ReceiveOnDamageReceived(InHealthComponent, DamageInfo);
}

void ACharacterBase::OnDied(UHealthComponent* InHealthComponent, FDamageInfo LastDamageInfo)
{
	UAnimMontage* MontageToPlay = FindTagedMontage(OnDeathMontages, LastDamageInfo.DamageIdentifier, DefaultDeathMontage);
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->StopAllMontages(0.2f);
		AnimInstance->Montage_Play(MontageToPlay);
	}

	AttackEnabled = false;
	SetStuned(true);

	ReceiveOnDied(InHealthComponent, LastDamageInfo);
}
