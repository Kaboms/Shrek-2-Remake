// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"
#include "CombatComponent.h"
#include "Core/GameplayTagsNative.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	UActorComponent* FindedCombatComponent = GetComponentByClass(UCombatComponent::StaticClass());
	if (IsValid(FindedCombatComponent))
	{
		CombatComponent = Cast<UCombatComponent>(FindedCombatComponent);
	}
}

bool ACharacterBase::GetIsAttack() const
{
	if (IsValid(CombatComponent))
	{
		return CombatComponent->bIsAttack;
	}
	return false;
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

void ACharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (IsValid(CombatComponent))
	{
		CombatComponent->InterruptAttack();
	}

	UpdateMovementMode();
}

bool ACharacterBase::GetIsStunned()
{
	return bStunned;
}

void ACharacterBase::SetStunned(bool bInStunned)
{
	bStunned = bInStunned;

	ReceiveOnStunned(bStunned);

	OnStunned.Broadcast(bStunned);
}

void ACharacterBase::UpdateMovementMode()
{
	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		if (WalkingSubMovementMode == EWalkingSubMovementMode::Wading)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWadeSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}
	}
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
	if (IsValid(CombatComponent))
	{
		CombatComponent->InterruptAttack();
	}

	if (HealthComponent->IsAlive())
	{
		SetStunned(true);

		if (!DamageInfo.ImpactPoint.IsZero())
		{
			if (FVector::DotProduct(GetActorForwardVector(), UKismetMathLibrary::Normal(DamageInfo.ImpactPoint - GetActorLocation())) >= 0)
			{
				DamageInfo.DamageIdentifier.AddTag(GameplayTagsNative::Damage_Direction_Front);
			}
			else
			{
				DamageInfo.DamageIdentifier.AddTag(GameplayTagsNative::Damage_Direction_Back);
			}
		}

		UAnimMontage* MontageToPlay = FindTagedMontage(OnDamageMontages, DamageInfo.DamageIdentifier, DefaultDamageMontage);

		if (MontageToPlay)
		{
			FOnMontageEnded OnMontageEndedDelegate;
			OnMontageEndedDelegate.BindLambda([this](UAnimMontage* AnimMontage, bool bInterrupted)
				{
					if (HealthComponent->IsAlive())
						SetStunned(false);
				});

			if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(MontageToPlay);
				AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, MontageToPlay);
			}
		}
		else
		{
			SetStunned(false);
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

	if (IsValid(CombatComponent))
	{
		CombatComponent->Deactivate();
	}

	SetStunned(true);

	ReceiveOnDied(InHealthComponent, LastDamageInfo);
}

void ACharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = CharacterTags;
}
