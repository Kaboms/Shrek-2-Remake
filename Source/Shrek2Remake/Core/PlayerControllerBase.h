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

protected:
	// Called for movement input
	void Move(const FInputActionValue& Value);

	void StopMove(const FInputActionValue& Value);

	// Called for looking input
	void Look(const FInputActionValue& Value);

	void Jump();
	void StopJumping();

	void AttackUse(const FInputActionValue& Value);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowMainMenu(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContextKMB;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> DefaultMappingContextGamepad;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputConfig> InputConfig;
};
