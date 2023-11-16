// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ExtendedAssetManager.h"
#include "Core/GameplayTagsNative.h"

void UExtendedAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading();

    InitializeNativeTags();
}

void UExtendedAssetManager::InitializeNativeTags()
{
    FGameplayTagsNative::InitializeNativeTags();
}