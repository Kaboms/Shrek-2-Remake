// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "ExtendedAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UExtendedAssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:
	virtual void StartInitialLoading() override;
};
