// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Save/SaveSystemService.h"
#include "Core/Save/SaveGameMain.h"
#include "Core/Save/SaveActorComponent.h"

#include "ServicesSubsystem.h"

USaveSystemService* USaveSystemService::Get(const UObject* WorldContextObject)
{
    return Cast<USaveSystemService>(UServicesSubsystem::K2_GetService(WorldContextObject, USaveSystemService::StaticClass()));
}

void USaveSystemService::SaveToDestroyed(USaveActorComponent* SaveComponent)
{
    if (IsValid(CurrentSave))
    {
        CurrentSave->DestroyedActors.Add(SaveComponent->GetGuid());
    }
}

bool USaveSystemService::IsActorDestroyed(USaveActorComponent* SaveComponent)
{
    if (IsValid(CurrentSave))
    {
        return CurrentSave->DestroyedActors.Contains(SaveComponent->GetGuid());
    }
    return false;
}
