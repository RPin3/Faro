// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MadnessWidget.generated.h"

/**
 * 
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class FARO_API UMadnessWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MadnessBar;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* MadnessText;

	UPROPERTY(BlueprintReadOnly)
	class AFaroCharacter* OwnerCharacter;

	FTimerHandle UpdateTimerHandle;

public:
	virtual void NativeConstruct() override;

	void InitializeWidget(AFaroCharacter* Character);
	UFUNCTION(BlueprintCallable)
	void UpdateMadness();
protected:

};
