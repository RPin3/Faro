// Copyright Epic Games, Inc. All Rights Reserved.

#include "FaroCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Faro.h"
#include "Interact.h"
#include "MadnessWidget.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

void AFaroCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StartMadnessTimer();
	}

	if (IsLocallyControlled())
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			if (MadnessWidgetClass)
			{
				UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(PC, MadnessWidgetClass);
				if (UMadnessWidget* MadnessWidget = Cast<UMadnessWidget>(WidgetInstance))
				{
					MadnessWidget->AddToViewport();
					MadnessWidget->InitializeWidget(this);

					MadnessWidgetInstance = MadnessWidget;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MadnessWidgetClass no asignado en el editor."));
			}

			if (SkillCheckClass)
			{
				UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(PC, SkillCheckClass);
				if (USkillCheck* SkillCheckWidget = Cast<USkillCheck>(WidgetInstance))
				{
					SkillCheckWidget->AddToViewport();
					//SkillCheckWidget->InitializeWidget(this);

					SkillCheckWidgetInstance = SkillCheckWidget;
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("MadnessWidgetClass no asignado en el editor."));
			}
			SkillCheckWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	
}


AFaroCharacter::AFaroCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Madness Locura para los que no saben ingles
	Madness = 100.f;
	MadnessDecreaseRate = 1.f;
	MadnessDecreaseInterval = 0.5f;
	bIsMad = false;

	//Aqui ya esta activa la replicacion primer aviso
	bReplicates = true;

}

void AFaroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFaroCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AFaroCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFaroCharacter::Look);
	}
	else
	{
		UE_LOG(LogFaro, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AFaroCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AFaroCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AFaroCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AFaroCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AFaroCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AFaroCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AFaroCharacter::StartMadnessTimer()
{
	if (GetWorld())
	{
		GetWorldTimerManager().SetTimer(
			MadnessTimerHandle,
			this,
			&AFaroCharacter::IncreaseMadness,
			MadnessDecreaseInterval,
			true
		);
	}
}



void AFaroCharacter::IncreaseMadness()
{
	if (!HasAuthority()) return;

	Madness -= MadnessDecreaseRate;
	Madness = FMath::Clamp(Madness, 0.f, 100.f);

	OnRep_Madness();

	if (Madness <= 0.f && !bIsMad)
	{
		HandleMadnessEmpty();
	}
}

void AFaroCharacter::HandleMadnessEmpty()
{
	if (!HasAuthority()) return;

	bIsMad = true;
	OnRep_bIsMad();
}

void AFaroCharacter::OnRep_bIsMad()
{

	UE_LOG(LogTemp, Warning, TEXT("%s se ha vuelto loco."), *GetName());
	//Aqui van las cosas que sucederan al volverte loco
}

void AFaroCharacter::OnRep_Madness()
{
	if (IsLocallyControlled() && MadnessWidgetInstance)
	{
		MadnessWidgetInstance->UpdateMadness();
	}
}

void AFaroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFaroCharacter, bIsMad);
	DOREPLIFETIME(AFaroCharacter, Madness);
	
}

void AFaroCharacter::ReciveInformation_Implementation(AActor* object)
{
	objectToInteract = object;
}

void AFaroCharacter::StartSkillCheck_Implementation()
{
	SkillCheckWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	SkillCheckWidgetInstance->NewSkillCheck();
}

void AFaroCharacter::Interact()
{
	if (objectToInteract->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	{
		IInteract::Execute_Interact(objectToInteract);
	}
}

