// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HelpersFunctionLibrary.h"

bool UHelpersFunctionLibrary::IsWithEditor()
{
#if WITH_EDITOR 
    return true;
#endif // WITH_EDITOR 

    return false;
}
