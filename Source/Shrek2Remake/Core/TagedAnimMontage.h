
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "TagedAnimMontage.generated.h"

USTRUCT(BlueprintType)
struct FTagedAnimMontage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer MontageTags;
};