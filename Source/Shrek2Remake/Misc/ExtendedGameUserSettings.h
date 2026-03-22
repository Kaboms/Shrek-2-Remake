// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "ExtendedGameUserSettings.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHREK2REMAKE_API UExtendedGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	/** Returns the game local machine settings (resolution, windowing mode, scalability settings, etc...) */
	UFUNCTION(BlueprintCallable, Category = Settings)
	static UExtendedGameUserSettings* GetExtendedGameUserSettings();

	UFUNCTION(BlueprintCallable)
	bool RunBenchmarkIfNeeded();

protected:
	UPROPERTY(config, BlueprintReadWrite)
	float MasterSoundVolume = 1;

	UPROPERTY(config, BlueprintReadWrite)
	float EffectsSoundVolume = 1;

	UPROPERTY(config, BlueprintReadWrite)
	float MusicSoundVolume = 1;

	UPROPERTY(config, BlueprintReadWrite)
	float DialogueSoundVolume = 1;

	UPROPERTY(config, BlueprintReadWrite)
	int32 AutoQuality = 5;
};
