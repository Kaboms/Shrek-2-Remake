// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "QualitySelector.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UQualitySelector : public UComboBoxString
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitDefaults(int32 QualityLevels = 5, int32 CurrentQualityLevel = 0);

	UFUNCTION(BlueprintCallable)
	void Init(TArray<FText> QualityLevelsNamesOverride, int32 QualityLevels = 5, int32 CurrentQualityLevel = 0);
};
