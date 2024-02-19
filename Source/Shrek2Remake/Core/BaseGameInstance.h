// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"


UENUM(BlueprintType)
enum class ECheckActorDestroyed : uint8
{
	Destroyed,
	Exist
};

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/** Returns the game instance object  */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject"))
	static class UBaseGameInstance* GetBaseGameInstance(const UObject* WorldContextObject);

	/** Returns the game instance object  */
	UFUNCTION(BlueprintPure, Category = "Game", meta = (WorldContext = "WorldContextObject", DeterminesOutputType = "GameInstanceClass"))
	static class UBaseGameInstance* GetGameInstanceByClass(const UObject* WorldContextObject, TSubclassOf<UBaseGameInstance> GameInstanceClass);
};
