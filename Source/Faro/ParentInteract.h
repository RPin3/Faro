
#pragma once

#include "CoreMinimal.h"
#include "Interact.h"
#include "SendInformation.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/DisableObject.h"
#include "ParentInteract.generated.h"

UCLASS()
class FARO_API AParentInteract : public AActor, public IInteract, public IDisableObject
{
	GENERATED_BODY()

public:
	AParentInteract();
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
protected:
	virtual void BeginPlay() override;
	
	AActor* PlayerTemp;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USphereComponent* sphere;
	UPROPERTY(EditAnywhere)
	UWidgetComponent* widget;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh;

	UPROPERTY()
	bool isComplete = false;

	virtual void Interact_Implementation();

	void ObjectComplete_Implementation();
};
