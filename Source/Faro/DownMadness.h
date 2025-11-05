// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DownMadness.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDownMadness : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARO_API IDownMadness
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Comunicación")
	void DowngradeMadness();
};
