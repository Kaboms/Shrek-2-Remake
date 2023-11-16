// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectHandlerInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UEffectHandlerInterface : public UInterface
{
    GENERATED_BODY()
};

class IEffectHandlerInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandlePush(FVector Direction, float Force);
};