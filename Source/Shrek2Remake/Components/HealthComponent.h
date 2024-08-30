// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "HealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpactPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer DamageIdentifier;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDiedSignature, UHealthComponent*, HealthComponent, FDamageInfo, LastDamageInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamagedSignature, UHealthComponent*, HealthComponent, FDamageInfo, DamageInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealedSignature, UHealthComponent*, HealthComponent, float, HealthAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthChangedSignature, UHealthComponent*, HealthComponent, float, NewHealthValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHREK2REMAKE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(float NewMaxHealth, bool AbjustHealthToMax = true);

	UFUNCTION(BlueprintCallable)
	void ApplyDamage(FDamageInfo DamageInfo);

	UFUNCTION(BlueprintCallable)
	void AddHealth(float HealthToAdd);

	UFUNCTION(BlueprintCallable)
	void HandleDeath(FDamageInfo LastDamageInfo);

	UFUNCTION(BlueprintCallable)
	void Die(FDamageInfo DeathDamageInfo);

	UFUNCTION(BlueprintPure)
	bool IsAlive() { return Health > 0; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanBeDamaged = true;

	// If true damage will not apply.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bImmortal = false;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FDiedSignature OnDied;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FDamagedSignature OnDamaged;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FHealedSignature OnHealed;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FHealthChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Event Dispatcher")
	FHealthChangedSignature OnMaxHealthChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health;
};
