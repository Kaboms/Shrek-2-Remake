// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "AnimNotifyState_Attack.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UAnimNotifyState_Attack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FGameplayTag GetSlotTag() { return SlotTag; }
	FGameplayTag GetAttackTag() { return AttackTag; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttackTag;
};
