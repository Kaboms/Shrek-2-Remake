// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/QualitySelector.h"

#define LOCTEXT_NAMESPACE "Scalability"

void UQualitySelector::InitDefaults(int32 QualityLevels, int32 CurrentQualityLevel)
{
	const TArray<FText> DefaultNames = TArray<FText>{ LOCTEXT("QualityLowLabel", "Low"), LOCTEXT("QualityMediumLabel", "Medium"), LOCTEXT("QualityHighLabel", "High"), LOCTEXT("QualityEpicLabel", "Epic"), LOCTEXT("QualityCineLabel", "Cinematic") };

	Init(DefaultNames, QualityLevels, CurrentQualityLevel);
}

void UQualitySelector::Init(TArray<FText> QualityLevelsNamesOverride, int32 QualityLevels, int32 CurrentQualityLevel)
{
	for (size_t i = 0; i < QualityLevels; i++)
	{
		AddOption(QualityLevelsNamesOverride[i].ToString());
	}

	SetSelectedIndex(CurrentQualityLevel);
}
