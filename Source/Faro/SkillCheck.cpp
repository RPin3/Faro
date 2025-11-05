// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCheck.h"

#include "DownMadness.h"
#include "SuccessSkillChecks.h"
#include "TimerManager.h"
#include "Components/Image.h"

void USkillCheck::NativeConstruct()
{
	Super::NativeConstruct();
	
	numOfSkillCheck = 1;
	currentSkillCheck = 0;
	toleranceMin = 1.0;
	toleranceMax = 0;
	middleAngle = -20;

	
}

void USkillCheck::NewSkillCheck()
{
	if (currentSkillCheck == numOfSkillCheck)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(moveTimer);
		}
		currentSkillCheck = 0;
	}
	else
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(
				moveTimer,
				this,
				&USkillCheck::MoveSkillCheck,
				0.01f,     
				true      
			);
		}

		skillCheckPoint = FMath::FRandRange(0.0f,1.0f);
		SkillCheck->GetDynamicMaterial()->SetScalarParameterValue(FName("PositionRotaton"),skillCheckPoint);


		inProcces = true;
		toleranceMin = skillCheckPoint*-360.0f-20.0f+20.0f;
		toleranceMax = skillCheckPoint*-360.0f-20.0f-20.0f;
	}
}

void USkillCheck::MoveSkillCheck()
{

	middleAngle += -1;

	if (middleAngle <=-380.0f)
	{
		middleAngle =-20.0f;
	}
	else
	{
		flecha->SetRenderTransformAngle(middleAngle);
	}
}

void USkillCheck::ComprobateSkillCheck()
{
	if (middleAngle <= toleranceMin)
	{
		if (middleAngle >= toleranceMax)
		{
			PlaySound(SkillCheckSoundSuccess);
			currentSkillCheck = 1;
			NewSkillCheck();
			if (playerActor->GetClass()->ImplementsInterface(USuccessSkillChecks::StaticClass()))
			{
				ISuccessSkillChecks::Execute_CompletSkillCheck(playerActor);
			} 
			this->SetVisibility(ESlateVisibility::Hidden);
			inProcces = false;
			
		}
		else
		{
			PlaySound(SkillCheckSoundFail);
			if (playerActor->GetClass()->ImplementsInterface(UDownMadness::StaticClass()))
			{
				IDownMadness::Execute_DowngradeMadness(playerActor);
			}
		}
	}
	else
	{
		PlaySound(SkillCheckSoundFail);
		if (playerActor->GetClass()->ImplementsInterface(UDownMadness::StaticClass()))
		{
			IDownMadness::Execute_DowngradeMadness(playerActor);
		}
	}
}
