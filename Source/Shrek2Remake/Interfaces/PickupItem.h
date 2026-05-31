// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickupItem.generated.h"

UENUM(BlueprintType)
enum class EPickupItemType : uint8
{
	None = 0,
	Throw,
	Use
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UPickupItem : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SHREK2REMAKE_API IPickupItem
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	EPickupItemType GetType() const;
};
