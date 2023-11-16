// (c) 2021 Sonic Terror


#include "Core/GameplayTagsNative.h"
#include "GameplayTagsManager.h"

FGameplayTagsNative FGameplayTagsNative::GameplayTags;

void FGameplayTagsNative::InitializeNativeTags()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(Manager);

	// Notify manager that we are done adding native tags.
	Manager.DoneAddingNativeTags();
}

void FGameplayTagsNative::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(Damage_Direction_Back, "Damage.Direction.Back", "Damage was received at back");
	AddTag(Damage_Direction_Front, "Damage.Direction.Front", "Damage was received at front");
}

void FGameplayTagsNative::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}