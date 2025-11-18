// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once


#include "CoreMinimal.h"
#include "DownMadness.h"
#include "Interact.h"
#include "GameFramework/Character.h"
#include "MadnessWidget.h"
#include "SendInformation.h"
#include "SkillCheck.h"
#include "StartNewSkillCheck.h"
#include "SuccessSkillChecks.h"
#include "Interfaces/PlayerInteractions.h"
#include "Logging/LogMacros.h"
#include "FaroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AFaroCharacter : public ACharacter, public ISendInformation, public IStartNewSkillCheck, public IDownMadness, public ISuccessSkillChecks, public IPlayerInteractions
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:
	
	virtual void BeginPlay() override;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

public:

	/** Constructor */
	AFaroCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Madness, VisibleAnywhere, BlueprintReadOnly, Category = "Madness")
	float Madness;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Madness")
	float MadnessDecreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Madness")
	float MadnessDecreaseInterval;

	UPROPERTY(ReplicatedUsing=OnRep_bIsMad, BlueprintReadOnly, Category="Madness")
	bool bIsMad;

	FTimerHandle MadnessTimerHandle;

	UFUNCTION()
	void IncreaseMadness();

	UFUNCTION()
	void HandleMadnessEmpty();
	
	UFUNCTION()
    void OnRep_DecreaseMadness(float Amount);

	UFUNCTION()
	void OnRep_bIsMad();

	UFUNCTION()
	void OnRep_Madness();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintPure, Category="Madness")
	float GetMadness() const { return Madness; }

	UFUNCTION(BlueprintCallable, Category="Madness")
	void StartMadnessTimer();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MadnessWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SkillCheckClass;

	
	UPROPERTY()
	UMadnessWidget* MadnessWidgetInstance;

	UPROPERTY()
	USkillCheck* SkillCheckWidgetInstance;
	
	virtual void ReciveInformation_Implementation(AActor* object) override;

	virtual void StartSkillCheck_Implementation();

	virtual void DowngradeMadness_Implementation();

	virtual void CompletSkillCheck_Implementation();

	virtual void TakePill_Implementation(float Num);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Interfaces")
	AActor* objectToInteract;

	UFUNCTION(BlueprintCallable, Category="Interact")
	void Interact();

	
	
};

