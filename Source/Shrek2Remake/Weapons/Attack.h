// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"

#include "Attack.generated.h"

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class SHREK2REMAKE_API UBaseAttack : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime);

	UFUNCTION(BlueprintImplementableEvent)
	void StartAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void EndAttack();

	UFUNCTION(BlueprintImplementableEvent)
	void BeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintImplementableEvent)
	void EndOverlap(AActor* OtherActor);

	virtual class UWorld* GetWorld() const override
	{
		if (GetOuter() == nullptr)
		{
			return nullptr;
		}

		// Special case for behavior tree nodes in the editor
		if (Cast<UPackage>(GetOuter()) != nullptr)
		{
			// GetOuter should return a UPackage and its Outer is a UWorld
			return Cast<UWorld>(GetOuter()->GetOuter());
		}

		// In all other cases...
		return GetOuter()->GetWorld();
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag AttackTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Force;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> AttackedActors;

	UPROPERTY(BlueprintReadOnly)
	class AWeapon* Weapon;
};