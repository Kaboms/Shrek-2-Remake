// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"

#include "Misc/Structs/LevelInfo.h"

#include "SaveGameMain.generated.h"

USTRUCT(BlueprintType)
struct FLevelSave : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName LevelName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName Checkpoint;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName Part;
};

UCLASS()
class SHREK2REMAKE_API USaveGameMain : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FLevelSave Level;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSet<FGuid> DestroyedActors;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 Coins;
};