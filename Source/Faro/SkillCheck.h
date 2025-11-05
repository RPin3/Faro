// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SkillCheck.generated.h"


UCLASS()
class FARO_API USkillCheck : public UUserWidget
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditAnywhere)
	int currentSkillCheck;

	UPROPERTY(EditAnywhere)
	int numOfSkillCheck;
	
	UPROPERTY(EditAnywhere)
	float middleAngle;

	UPROPERTY(EditAnywhere)
	float skillCheckPoint;

	UPROPERTY(EditAnywhere)
	float toleranceMin;

	UPROPERTY(EditAnywhere)
	float toleranceMax;
	
	UPROPERTY(meta = (BindWidget))
	UImage* flecha;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillCheck;
	
	UFUNCTION(CallInEditor)
	void NewSkillCheck();

	UFUNCTION(CallInEditor)
	void MoveSkillCheck();

	UFUNCTION(BlueprintCallable, category = "Interact")
	void ComprobateSkillCheck();

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SkillCheckSoundFail;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* SkillCheckSoundSuccess;

private:
	FTimerHandle moveTimer;


protected:
	virtual void NativeConstruct() override;
};
