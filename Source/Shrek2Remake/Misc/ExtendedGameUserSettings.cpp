// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedGameUserSettings.h"
#include "ComponentRecreateRenderStateContext.h"

UExtendedGameUserSettings* UExtendedGameUserSettings::GetExtendedGameUserSettings()
{
    return Cast<UExtendedGameUserSettings>(GetGameUserSettings());
}

void UExtendedGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
    {
        // Push recreate render state context to force single recreate instead of multiple recreates for each changed cvar
        FGlobalComponentRecreateRenderStateContext Context;

        if (GetLastConfirmedScreenResolution() != GetScreenResolution() || GetFullscreenMode() != GetLastConfirmedFullscreenMode())
        {
            ApplyResolutionSettings(bCheckForCommandLineOverrides);
        }
        ApplyNonResolutionSettings();
    }
    RequestUIUpdate();

    SaveSettings();
}

bool UExtendedGameUserSettings::RunBenchmarkIfNeeded()
{
    if (AutoQuality == 5 && (LastCPUBenchmarkResult == -1 || LastGPUBenchmarkResult == -1))
    {
        RunHardwareBenchmark();
        ApplyHardwareBenchmarkResults();

        return true;
    }
    return false;
}
