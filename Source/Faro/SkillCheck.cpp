// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillCheck.h"
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
