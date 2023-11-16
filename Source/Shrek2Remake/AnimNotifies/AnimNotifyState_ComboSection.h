// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "AnimNotifyState_ComboSection.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UAnimNotifyState_ComboSection : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FName GetNextAttackSection() { return NextAttackSection; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NextAttackSection;
};
