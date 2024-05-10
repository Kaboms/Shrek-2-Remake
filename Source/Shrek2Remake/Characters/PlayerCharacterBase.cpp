// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacterBase.h"
#include "Core/PlayerControllerBase.h"
#include "Weapons/Weapon.h"
#include "Components/ClimbingComponent.h"
#include "EnhancedInputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// All player characters has double jump
	JumpMaxCount = 2;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bInheritPitch = true;
	CameraBoom->bInheritYaw = true;
	CameraBoom->bInheritRoll = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Setup movement params
	FallingCurrentRollSpeed= 0;
	MaxFallingRoll = 5;
	FallingRollSpeed = 25;
	FallingTargetRollSpeed = 0;

	GetCharacterMovement()->bNotifyApex = true;
	bCanDoubleJumpOnFalling = false;
	bJumpApexReached = false;
}

void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerController->PlayerCameraManager)
	{
		// Limit camera rotation
		PlayerController->PlayerCameraManager->ViewPitchMin = -80;
		PlayerController->PlayerCameraManager->ViewPitchMax = 45;
	}
}

void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddRollOnFalling(DeltaTime);
}

void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		PlayerController = Cast<APlayerControllerBase>(Controller);

		UInputConfig* InputConfig = PlayerController->GetInputConfig();

		//Jumping
		EnhancedInputComponent->BindAction(InputConfig->JumpInputAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::Jump);
		EnhancedInputComponent->BindAction(InputConfig->JumpInputAction, ETriggerEvent::Completed, this, &APlayerCharacterBase::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(InputConfig->MoveInputAction, ETriggerEvent::Triggered, this, &APlayerCharacterBase::OnMoveInput);

		//Attack/Use
		EnhancedInputComponent->BindAction(InputConfig->AttackUseInputAction, ETriggerEvent::Started, this, &APlayerCharacterBase::AttackUse);
	}
}

void APlayerCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UActorComponent* FindedClimbingComponent = GetComponentByClass(UClimbingComponent::StaticClass());
	if (IsValid(FindedClimbingComponent))
	{
		ClimbingComponent = Cast<UClimbingComponent>(FindedClimbingComponent);
	}
}

void APlayerCharacterBase::Attack()
{
	if (!CanAttack())
		return;

	ReceiveAttack();
}

bool APlayerCharacterBase::CanAttack_Implementation() const
{
	return !bStunned && !bInAction && GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking
		&& !GetMovementComponent()->IsFalling();
		/*&& EWalkingSubMovementMode != EWalkingSubMovementMode::Wading original game has ability to attack in water, but this look weird*/
}

void APlayerCharacterBase::OnMoveInput(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// Get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Move(ForwardDirection, RightDirection, MovementVector);
}

void APlayerCharacterBase::Move(FVector ForwardDirection, FVector RightDirection, FVector2D MovementVector)
{
	if (bStunned)
		return;

	if (IsValid(ClimbingComponent) && ClimbingComponent->CanHandleMovement())
	{
		// Input handled by climbing component
		return;
	}

	// Disable movement on attack
	if (!GetIsAttack())
	{
		// Add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

bool APlayerCharacterBase::IsWading() const
{
	return GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking && WalkingSubMovementMode == EWalkingSubMovementMode::Wading;
}

void APlayerCharacterBase::Jump()
{
	if (bStunned)
		return;

	if (IsWading())
	{
		ReceiveOnWadingJump();
	}

	Super::Jump();
}

void APlayerCharacterBase::AddRollOnFalling(const float& DeltaTime)
{
	if (GetMovementComponent()->IsFalling())
	{
		FRotator MeshRotation = GetMesh()->GetRelativeRotation();

		FVector MovementVector = GetTransform().InverseTransformVector(GetLastMovementInputVector());

		if (FMath::Abs(MovementVector.Y) > 0.2)
		{
			// Add roll
			if (MeshRotation.Pitch < MaxFallingRoll && MeshRotation.Pitch > -MaxFallingRoll)
			{
				FallingTargetRollSpeed = MovementVector.Y * FallingRollSpeed;
			}
			else
			{
				// Reached MaxFlyingRoll so we stop rolling
				FallingCurrentRollSpeed = 0;
			}
		}
		else
		{
			// Try reset rolling if here is no input from player
			if (MeshRotation.Pitch < 0.5 && MeshRotation.Pitch > -0.5)
			{
				// Mesh rotation is nearly zero, so we can stop
				FallingTargetRollSpeed = 0;
				FallingCurrentRollSpeed = 0;
			}
			else
			{
				// Set TargetRollSpeed to smoothly reset roll to zero
				FallingTargetRollSpeed = MeshRotation.Pitch * -FallingRollSpeed;
			}
		}

		// Smoothly interpolate roll speed
		FallingCurrentRollSpeed = FMath::FInterpTo(FallingCurrentRollSpeed, FallingTargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

		FRotator Rotation = FRotator::ZeroRotator;
		Rotation.Pitch = FallingCurrentRollSpeed * DeltaTime;
		GetMesh()->AddRelativeRotation(Rotation);
	}
}

void APlayerCharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	ResetFallingRoll();
}

void APlayerCharacterBase::NotifyJumpApex()
{
	Super::NotifyJumpApex();

	bJumpApexReached = true;
}

void APlayerCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UpdateMovementMode();
}

void APlayerCharacterBase::UpdateMovementMode()
{
	ResetFallingRoll();

	if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
	{
		if (WalkingSubMovementMode == EWalkingSubMovementMode::Wading)
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWadeSpeed;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		}
	}
}

void APlayerCharacterBase::RotateYawTo(FVector Point)
{
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw = FRotationMatrix::MakeFromX(Point - GetActorLocation()).Rotator().Yaw;
	SetActorRotation(Rotation);
}

bool APlayerCharacterBase::CanJumpInternal_Implementation() const
{
	if (GetIsAttack() || IsWading())
	{
		return false;
	}

	const bool bCanJump = Super::CanJumpInternal_Implementation();
	if (bCanDoubleJumpOnFalling || !bCanJump)
	{
		return bCanJump;
	}

	return !bJumpApexReached;
}


void APlayerCharacterBase::ResetFallingRoll()
{
	// Reset mesh rotation to zero when landed
	FRotator CurrentRotation = GetMesh()->GetRelativeRotation();
	CurrentRotation.Pitch = 0;
	GetMesh()->SetRelativeRotation(CurrentRotation);

	FallingCurrentRollSpeed = 0;
	FallingTargetRollSpeed = 0;

	bJumpApexReached = false;
	GetCharacterMovement()->bNotifyApex = true;
}