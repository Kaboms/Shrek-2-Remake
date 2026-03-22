// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceBase.h"

#include "SaveSystemService.generated.h"

class USaveGameMain;
class USaveActorComponent;

/**
 * 
 */
UCLASS()
class SHREK2REMAKE_API USaveSystemService : public UServiceBase
{
	GENERATED_BODY()

public:
	static USaveSystemService* Get(const UObject* WorldContextObject);

	void SaveToDestroyed(USaveActorComponent* SaveComponent);
	bool IsActorDestroyed(USaveActorComponent* SaveComponent);

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FName CurrentSaveSlotName;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TObjectPtr<USaveGameMain> CurrentSave;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<FName, USaveGameMain*> CachedSaveSlots;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncSaveStarted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Event Dispatcher")
	FAsyncSaveStarted AsyncSaveStarted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncSaveCompleted);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Event Dispatcher")
	FAsyncSaveCompleted AsyncSaveCompleted;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveLoaded);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Event Dispatcher")
	FSaveLoaded SaveLoaded;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveCalled);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, EditDefaultsOnly, Category = "Event Dispatcher")
	FOnSaveCalled OnSaveCalled;

};
