// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Characters/CharacterBase.h"
#include "Components/HealthComponent.h"

#include "Components/CapsuleComponent.h"

AWeapon::AWeapon() : Super()
{
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
    RootComponent = CapsuleComponent;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    Character = Cast<ACharacterBase>(GetInstigator());

    for (UBaseAttack* BaseAttack : InstancedAttacks)
    {
        BaseAttack->Weapon = this;
        Attacks.Add(BaseAttack->AttackTag, BaseAttack);
    }

    Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bAttackStarted && CurrentAttack)
    {
        CurrentAttack->Tick(DeltaTime);
    }
}

void AWeapon::OnAttackStartedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = true;

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

    CurrentAttack = nullptr;

    if (UBaseAttack** FindedAttack = Attacks.Find(AttackTag))
    {
        CurrentAttack = *FindedAttack;
    }
    if (IsValid(CurrentAttack))
    {
        CurrentAttack->StartAttack();
    }

    ReceiveOnAttackStartedNotify(AttackTag);
}

void AWeapon::OnAttackEndedNotify(FGameplayTag AttackTag)
{
    bAttackStarted = false;

    if (CurrentAttack == nullptr)
        return;

    CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

    if (UBaseAttack** FindedAttack = Attacks.Find(AttackTag))
    {
        if ((*FindedAttack) == CurrentAttack)
        {
            CurrentAttack->EndAttack();
            CurrentAttack = nullptr;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("New attack started before previous received AttackEndedNotify"));
        }
    }

    ReceiveOnAttackEndedNotify(AttackTag);
}

void AWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if (!bAttackStarted || GetInstigator() == OtherActor)
        return;

    if (IsValid(CurrentAttack))
    {
        CurrentAttack->BeginOverlap(OtherActor);
    }

    Super::NotifyActorBeginOverlap(OtherActor);
}

void AWeapon::NotifyActorEndOverlap(AActor* OtherActor)
{
    if (GetInstigator() == OtherActor)
        return;

    if (IsValid(CurrentAttack))
    {
        CurrentAttack->EndOverlap(OtherActor);
    }

    Super::NotifyActorEndOverlap(OtherActor);
}
