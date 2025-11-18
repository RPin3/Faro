// Fill out your copyright notice in the Description page of Project Settings.


#include "Pill.h"

#include "Interfaces/PlayerInteractions.h"


// Sets default values
APill::APill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APill::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APill::Interact_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pastilla recogida"));
	}

	if (PlayerTemp->GetClass()->ImplementsInterface(UPlayerInteractions::StaticClass()))
	{
		IPlayerInteractions::Execute_TakePill(PlayerTemp, 25.0f);
	}	
}


