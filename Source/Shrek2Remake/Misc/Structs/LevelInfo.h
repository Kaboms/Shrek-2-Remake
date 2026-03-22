
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CommonButtonBase.h"

#include "LevelInfo.generated.h"

class UWorld;

USTRUCT(BlueprintType)
struct FLevelCheckpoint
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCommonButtonStyle> ButtonStyle;
};

USTRUCT(BlueprintType)
struct FLevelInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FText DisplayName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UCommonButtonStyle> DefaultButtonStyle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, FLevelCheckpoint> Checkpoints;
};