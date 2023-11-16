// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameInstance.h"

#include "Kismet/GameplayStatics.h"

UBaseGameInstance* UBaseGameInstance::GetBaseGameInstance(const UObject* WorldContextObject)
{
    return Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
}

void UBaseGameInstance::CheckActorDestroyed(AActor* Actor, ECheckActorDestroyed& Branches)
{
    Branches = ReceiveCheckActorDestroyed(Actor) ? ECheckActorDestroyed::Destroyed : ECheckActorDestroyed::Exist;
}
