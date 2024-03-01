// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/HealthComponent.h"

#include "Core/TagedAnimMontage.h"

#include "CharacterBase.generated.h"

class UFightComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStunnedSignature, bool, IsStunned);

UCLASS()
class SHREK2REMAKE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintPure)
	bool GetIsAttack() const;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "On Death Montage Ended"))
	void OnDeathMontageEnded();

	static UAnimMontage* FindTagedMontage(TArray<FTagedAnimMontage>& Montages, FGameplayTagContainer MontageTags, UAnimMontage* DefaultMontage);

	UFUNCTION(BlueprintGetter)
	bool GetIsStunned();

	UFUNCTION(BlueprintCallable)
	void SetStunned(bool bStunned);

protected:
	UFUNCTION()
	void OnPlayMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	UFUNCTION()
	void OnDamageReceived(UHealthComponent* InHealthComponent, FDamageInfo DamageInfo);

	UFUNCTION()
	void OnDied(UHealthComponent* InHealthComponent, FDamageInfo LastDamageInfo);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "On Damage Received"))
	void ReceiveOnDamageReceived(UHealthComponent* InHealthComponent, FDamageInfo DamageInfo);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "On Died"))
	void ReceiveOnDied(UHealthComponent* InHealthComponent, FDamageInfo LastDamageInfo);

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "On Stuned"))
	void ReceiveOnStunned(bool bInStunned);

public:
	UPROPERTY(BlueprintAssignable, Category = "Event Dispatched")
	FStunnedSignature OnStunned;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	UAnimMontage* DefaultDamageMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	TArray<FTagedAnimMontage> OnDamageMontages;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	UAnimMontage* DefaultDeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	TArray<FTagedAnimMontage> OnDeathMontages;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetIsStunned)
	bool bStunned = false;
};
