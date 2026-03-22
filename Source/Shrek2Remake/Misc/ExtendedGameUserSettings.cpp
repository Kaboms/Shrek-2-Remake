// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtendedGameUserSettings.h"

UExtendedGameUserSettings* UExtendedGameUserSettings::GetExtendedGameUserSettings()
{
    return Cast<UExtendedGameUserSettings>(GetGameUserSettings());
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
