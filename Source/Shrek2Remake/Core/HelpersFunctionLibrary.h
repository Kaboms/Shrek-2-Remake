// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HelpersFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UHelpersFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure)
	static bool IsWithEditor();

	UFUNCTION(BlueprintPure)
	static bool DoesTargetMeetTagRequirements(const FGameplayTagRequirements& SourceTagReqs, const FGameplayTagContainer& TargetTags);
};
