// (c) 2021 Sonic Terror

#pragma once

#include "GameplayTagContainer.h"

/**
 * Inspired by FLyraGameplayTags
 */
struct FGameplayTagsNative
{

public:
	static const FGameplayTagsNative& Get() { return GameplayTags; }

	static void InitializeNativeTags();

protected:
	void AddAllTags(UGameplayTagsManager& Manager);
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

public:
	FGameplayTag Damage_Direction_Back;
	FGameplayTag Damage_Direction_Front;

	FGameplayTag Character_Player;
	FGameplayTag Character_Enemy;

private:
	static FGameplayTagsNative GameplayTags;
};
