// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "Characters/PlayerCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

void APlayerControllerBase::SetupInputComponent()
{
    Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContextKMB, 0);
		Subsystem->AddMappingContext(DefaultMappingContextGamepad, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//Looking
		EnhancedInputComponent->BindAction(InputConfig->LookInputAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Look);

		//Pause
		EnhancedInputComponent->BindAction(InputConfig->PauseInputAction, ETriggerEvent::Started, this, &APlayerControllerBase::ShowMainMenu);
	}
}

UInputConfig* APlayerControllerBase::GetInputConfig()
{
	return InputConfig;
}

void APlayerControllerBase::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}
