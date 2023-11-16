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
		//Jumping
		EnhancedInputComponent->BindAction(InputConfig->JumpInputAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Jump);
		EnhancedInputComponent->BindAction(InputConfig->JumpInputAction, ETriggerEvent::Completed, this, &APlayerControllerBase::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(InputConfig->MoveInputAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Move);
		EnhancedInputComponent->BindAction(InputConfig->MoveInputAction, ETriggerEvent::Completed, this, &APlayerControllerBase::StopMove);

		//Looking
		EnhancedInputComponent->BindAction(InputConfig->LookInputAction, ETriggerEvent::Triggered, this, &APlayerControllerBase::Look);

		//Attack/Use
		EnhancedInputComponent->BindAction(InputConfig->AttackUseInputAction, ETriggerEvent::Started, this, &APlayerControllerBase::AttackUse);

		//Pause
		EnhancedInputComponent->BindAction(InputConfig->PauseInputAction, ETriggerEvent::Started, this, &APlayerControllerBase::ShowMainMenu);
	}
}

void APlayerControllerBase::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetCharacter()))
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		PlayerCharacter->Move(ForwardDirection, RightDirection, MovementVector);
	}
}

void APlayerControllerBase::StopMove(const FInputActionValue& Value)
{
	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetCharacter()))
	{
		PlayerCharacter->StopMovement();
	}
}

void APlayerControllerBase::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddYawInput(LookAxisVector.X);
	AddPitchInput(-LookAxisVector.Y);
}

void APlayerControllerBase::Jump()
{
	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetCharacter()))
	{
		PlayerCharacter->Jump();
	}
}

void APlayerControllerBase::StopJumping()
{
	if (ACharacter* PlayerCharacter = GetCharacter())
	{
		PlayerCharacter->StopJumping();
	}
}

void APlayerControllerBase::AttackUse(const FInputActionValue& Value)
{
	if (APlayerCharacterBase* PlayerCharacter = Cast<APlayerCharacterBase>(GetCharacter()))
	{
		PlayerCharacter->AttackUse();
	}
}
