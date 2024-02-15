// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterBase.h"

#include "Input/InputConfig.h"
#include "PlayerCharacterBase.generated.h"

UENUM(BlueprintType)
enum class EWalkingSubMovementMode : uint8
{
	None,
	Wading
};

class AWeapon;

UCLASS()
class SHREK2REMAKE_API APlayerCharacterBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacterBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

	virtual void Landed(const FHitResult& Hit) override;

	virtual void NotifyJumpApex() override;

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

	UFUNCTION(BlueprintCallable)
	void UpdateMovementMode();

	UFUNCTION(BlueprintCallable)
	void RotateYawTo(FVector Point);

	UFUNCTION(BlueprintImplementableEvent)
	void AttackUse();

	UFUNCTION(BlueprintCallable)
	void Attack();

	void OnMoveInput(const FInputActionValue& Value);

	void Move(FVector ForwardDirection, FVector RightDirection, FVector2D MovementVector);

	UFUNCTION(BlueprintCallable)
	bool IsWading() const;

	virtual void Jump() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool CanJumpInternal_Implementation() const override;

	void ResetFallingRoll();

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "Attack"))
	void ReceiveAttack();

	UFUNCTION(BlueprintImplementableEvent, Meta = (DisplayName = "OnWadingJump"))
	void ReceiveOnWadingJump();

private:
	// In the original Shrek 2 game player character has roll to movement direction on falling
	void AddRollOnFalling(const float& DeltaTime);

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class APlayerControllerBase> PlayerController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UClimbingComponent* ClimbingComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	EWalkingSubMovementMode WalkingSubMovementMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	float FallingCurrentRollSpeed;

	// When falling we can roll character. This opportunity was in the original game
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	int32 MaxFallingRoll;

	// Speed to roll the character when falling
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	int32 FallingRollSpeed;

	// Can character make jump when falling. When false we cannot make jump after reached jump apex or on falling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bCanDoubleJumpOnFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	int MaxWalkSpeed = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	int MaxWadeSpeed = 300;

	/*
	* Uses to stop jump ability after reach jump apex
	* So player cannot jump on falling
	*/
	UPROPERTY(BlueprintReadWrite)
	bool bJumpApexReached;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;

private:
	// Uses to calculate roll speed on falling
	float FallingTargetRollSpeed;
};
