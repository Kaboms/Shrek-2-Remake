// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/HelpersFunctionLibrary.h"
#include "GameplayEffectTypes.h"

bool UHelpersFunctionLibrary::IsWithEditor()
{
#if WITH_EDITOR 
    return true;
#else
    return false;
#endif // WITH_EDITOR 
}

bool UHelpersFunctionLibrary::DoesTargetMeetTagRequirements(const FGameplayTagRequirements& SourceTagReqs, const FGameplayTagContainer& TargetTags)
{
    return SourceTagReqs.RequirementsMet(TargetTags);
}

FString UHelpersFunctionLibrary::GetAppVersion()
{
	FString AppVersion;
	GConfig->GetString(
		TEXT("/Script/EngineSettings.GeneralProjectSettings"),
		TEXT("ProjectVersion"),
		AppVersion,
		GGameIni
	);

	return AppVersion;
}