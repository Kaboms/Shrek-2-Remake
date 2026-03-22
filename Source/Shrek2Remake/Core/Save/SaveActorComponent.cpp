// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Save/SaveActorComponent.h"
#include "Core/Save/SaveSystemService.h"

USaveActorComponent::USaveActorComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USaveActorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (USaveSystemService* SaveSystemService = USaveSystemService::Get(this))
	{
		if (SaveSystemService->IsActorDestroyed(this))
		{
			GetOwner()->Destroy();
		}
	}
}

void USaveActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USaveActorComponent::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	if (IsTemplate())
	{
		if (Guid.IsValid())
		{
			Guid.Invalidate();
		}
	}
	else if (Ar.IsPersistent() && !Guid.IsValid())
	{
		Guid = FGuid::NewGuid();
	}
}

void USaveActorComponent::SaveToDestroyed()
{
	if (USaveSystemService* SaveSystemService = USaveSystemService::Get(this))
	{
		SaveSystemService->SaveToDestroyed(this);
	}
}

