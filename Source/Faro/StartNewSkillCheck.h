// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StartNewSkillCheck.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UStartNewSkillCheck : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FARO_API IStartNewSkillCheck
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Comunicación")
	void StartSkillCheck();
};
