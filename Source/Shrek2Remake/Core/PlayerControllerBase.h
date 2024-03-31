// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/InputConfig.h"
#include "InputActionValue.h"
#include "PlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Meta = (DeterminesOutputType="PlayerControllerClass", WorldContext = "WorldContextObject"))
	static APlayerController* GetPlayerControllerByClass(const UObject* WorldContextObject, TSubclassOf<APlayerController> PlayerControllerClass);

	UFUNCTION(BlueprintGetter)
	UInputConfig* GetInputConfig();

protected:
	// Called for looking input
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPauseMenu(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContextKMB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContextGamepad;

	UPROPERTY(EditDefaultsOnly, BlueprintGetter = GetInputConfig, Category = Input)
	TObjectPtr<UInputConfig> InputConfig;
};
