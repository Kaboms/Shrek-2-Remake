// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FightComponent.h"
#include "Characters/CharacterBase.h"
#include "Weapons/Weapon.h"

// Sets default values for this component's properties
UFightComponent::UFightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UFightComponent::BeginPlay()
{
    Character = Cast<ACharacterBase>(GetOwner());

	Super::BeginPlay();
}

// Called every frame
void UFightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UFightComponent::StartWeaponCombo(FGameplayTag SlotTag, FGameplayTag ComboTag)
{
    if (bIsAttack)
    {
        bMakeNextAttack = true;
    }
    else if (UWeaponSlot** WeaponSlotPtr = WeaponSlots.Find(SlotTag))
    {
        UWeaponSlot* WeaponSlot = *WeaponSlotPtr;
        AWeapon* Weapon = WeaponSlot->GetCurrentWeapon();
        if (IsValid(Weapon))
        {
            Weapon->StartCombo(ComboTag);

            bIsAttack = true;
        }
    }
}

void UFightComponent::OnComboSectionStartedNotify(UAnimNotifyState_ComboSection* ComboSectionNS)
{
    ReceiveOnComboSectionStartedNotify(ComboSectionNS);
}

void UFightComponent::OnComboSectionEndedNotify(UAnimNotifyState_ComboSection* ComboSectionNS)
{
    if (bMakeNextAttack)
    {
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
        AnimInstance->Montage_JumpToSection(ComboSectionNS->GetNextAttackSection(), AnimInstance->GetCurrentActiveMontage());

        bMakeNextAttack = false;
    }

    ReceiveOnComboSectionEndedNotify(ComboSectionNS);
}