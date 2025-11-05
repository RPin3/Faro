// Fill out your copyright notice in the Description page of Project Settings.


#include "MadnessWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "FaroCharacter.h"

void UMadnessWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMadnessWidget::InitializeWidget(AFaroCharacter* Character)
{
	if (!Character) return;

	OwnerCharacter = Character;

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(
			UpdateTimerHandle,
			this,
			&UMadnessWidget::UpdateMadness,
			0.1f, // actualización cada 0.1s
			true
		);
	}
}


void UMadnessWidget::UpdateMadness()
{
	if (!OwnerCharacter || !MadnessBar) return;

	float MadnessValue = OwnerCharacter->GetMadness() / 100.f;
	MadnessBar->SetPercent(MadnessValue);

	if (MadnessText)
	{
		MadnessText->SetText(FText::AsNumber(FMath::RoundToInt(MadnessValue * 100.f)));
	}
}
