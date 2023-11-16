// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "AnimNotifies/AnimNotifyState_ComboSection.h"
#include "FightComponent.generated.h"

class AWeapon;
class UWeaponSlot;
class ACharacterBase;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHREK2REMAKE_API UFightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartWeaponCombo(FGameplayTag SlotTag, FGameplayTag ComboTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartCombo(UAnimMontage* ComboMontage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InterruptAttack();

	UFUNCTION(BlueprintCallable)
	void OnComboSectionStartedNotify(UAnimNotifyState_ComboSection* ComboSectionNS);

	UFUNCTION(BlueprintCallable)
	void OnComboSectionEndedNotify(UAnimNotifyState_ComboSection* ComboSectionNS);

protected:
	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnComboSectionStartedNotify"))
	void ReceiveOnComboSectionStartedNotify(const UAnimNotifyState_ComboSection* ComboSectionNS);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnComboSectionEndedNotify"))
	void ReceiveOnComboSectionEndedNotify(const UAnimNotifyState_ComboSection* ComboSectionNS);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsAttack = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ACharacterBase* Character;

	UPROPERTY(BlueprintReadWrite)
	bool bMakeNextAttack = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TMap<FGameplayTag, UWeaponSlot*> WeaponSlots;
};
