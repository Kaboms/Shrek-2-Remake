// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "Attack.h"
#include "AnimNotifies/AnimNotifyState_Attack.h"
#include "Weapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackEnded);

class ACharacterBase;

UCLASS(BlueprintType, EditInlineNew)
class UWeaponSlot : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	AWeapon* GetCurrentWeapon()
	{
		if (Weapons.IsValidIndex(CurrentWeaponIndex))
		{
			return Weapons[CurrentWeaponIndex];
		}

		return nullptr;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MeshSlotName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<AWeapon>> DefaultWeapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AWeapon*> Weapons;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentWeaponIndex = 0;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SHREK2REMAKE_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnAttackStartedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintCallable)
	void OnAttackEndedNotify(FGameplayTag AttackTag);

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartCombo(FGameplayTag ComboTag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnAttackStartedNotify"))
	void ReceiveOnAttackStartedNotify(FGameplayTag AttackTag);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnAttackEndedNotify"))
	void ReceiveOnAttackEndedNotify(FGameplayTag AttackTag);

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacterBase* Character;

	UPROPERTY(BlueprintReadWrite)
	bool bAttackStarted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UBaseAttack*> InstancedAttacks;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, UBaseAttack*> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, UAnimMontage*> ComboMontages;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBaseAttack> CurrentAttack;
};
