// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

#include "Kismet/GameplayStatics.h"

UBaseGameInstance* UBaseGameInstance::GetBaseGameInstance(const UObject* WorldContextObject)
{
    return Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

UBaseGameInstance* UBaseGameInstance::GetGameInstanceByClass(const UObject* WorldContextObject, TSubclassOf<UBaseGameInstance> GameInstanceClass)
{
    UBaseGameInstance* BaseGameInstance = GetBaseGameInstance(WorldContextObject);
    ensure(BaseGameInstance->GetClass() == GameInstanceClass);

    return BaseGameInstance;
}
