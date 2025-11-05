

#include "ParentInteract.h"

#include "StartNewSkillCheck.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"




AParentInteract::AParentInteract()
{
	PrimaryActorTick.bCanEverTick = true;


	sphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = sphere;

	sphere->InitSphereRadius(100.0f);
	sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);   // No bloquea, solo detecta
	sphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	sphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	sphere->SetGenerateOverlapEvents(true);


	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetupAttachment(RootComponent);

	
	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
	widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	widget->SetupAttachment(RootComponent);
	widget->SetWidgetSpace(EWidgetSpace::Screen);  // o EWidgetSpace::World
	widget->SetDrawSize(FVector2D(200.f, 100.f));  // tamaño del widget

	
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBP(TEXT("/Game/UI/WBP_Interact"));
	if (WidgetBP.Succeeded())
	{
		widget->SetWidgetClass(WidgetBP.Class);
	}

	
	sphere->OnComponentBeginOverlap.AddDynamic(this, &AParentInteract::OnOverlapBegin);
	sphere->OnComponentEndOverlap.AddDynamic(this, &AParentInteract::OnOverlapEnd);
}


void AParentInteract::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	widget->SetVisibility(true);
	PlayerTemp = OtherActor;
	if (OtherActor->GetClass()->ImplementsInterface(USendInformation::StaticClass()))
	{
		ISendInformation::Execute_ReciveInformation(OtherActor, this);
	}
}

void AParentInteract::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	widget->SetVisibility(false);
	PlayerTemp = OtherActor;
	if (PlayerTemp->GetClass()->ImplementsInterface(USendInformation::StaticClass()))
	{
		ISendInformation::Execute_ReciveInformation(PlayerTemp, nullptr);
	}
}


void AParentInteract::BeginPlay()
{
	Super::BeginPlay();
	
}


void AParentInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AParentInteract::Interact_Implementation()
{
	if (isComplete == false)
	{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Interactuaron conmigo"));
	}

	if (PlayerTemp->GetClass()->ImplementsInterface(UStartNewSkillCheck::StaticClass()))
	{
		IStartNewSkillCheck::Execute_StartSkillCheck(PlayerTemp);
	}
	}
}

void AParentInteract::ObjectComplete_Implementation()
{
	widget->SetHiddenInGame(true);
	isComplete = true;
}

