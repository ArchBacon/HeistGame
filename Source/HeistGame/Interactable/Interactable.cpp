// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "HeistGame/HeistGame.h"
#include "HeistGame/UI/Widgets/HeistProgressBar.h"

AInteractable::AInteractable()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	BoxComponent->SetCollisionProfileName(Interaction_ProfileName);
	BoxComponent->CanCharacterStepUpOn = ECB_No;
	RootComponent = BoxComponent;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(BoxComponent);

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	Widget->SetDrawAtDesiredSize(true);
	Widget->SetUsingAbsoluteRotation(true);
	Widget->SetWorldRotation(FRotator(90.f, 180.f, 0.f));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/HeistGame/UI/WBP_ProgressBar"));
	if (WidgetClassFinder.Succeeded())
	{
		Widget->SetWidgetClass(WidgetClassFinder.Class);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> InactiveMatFinder(TEXT("MaterialInstanceConstant'/Game/HeistGame/Materials/M_grey.M_grey'"));
	if (InactiveMatFinder.Succeeded())
	{
		InactiveMaterial = InactiveMatFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ActiveMatFinder(TEXT("MaterialInstanceConstant'/Game/HeistGame/Materials/M_Green.M_Green'"));
	if (ActiveMatFinder.Succeeded())
	{
		ActiveMaterial = ActiveMatFinder.Object;
	}

	bIsActive = true;
	bSaveProgress = false;
	HoldDuration = 2.f;
	SavedProgress = 0.f;
}

void AInteractable::BeginPlay()
{
	Super::BeginPlay();

	SetWidgetVisibility(ESlateVisibility::Hidden);
}

void AInteractable::UpdateMaterialsOnMesh() const
{
	for (int i = 0; i < Mesh->GetNumMaterials(); i++)
	{
		Mesh->SetMaterial(i, bIsActive ? ActiveMaterial : InactiveMaterial);
	}

	OnMaterialsUpdated.Broadcast();
}

// Place the widget on top of the collision box
void AInteractable::UpdateWidgetLocation() const
{
	FVector WidgetLocation = GetActorLocation();
	WidgetLocation.Z += BoxComponent->GetScaledBoxExtent().Z + 10;
	
	Widget->SetWorldLocation(WidgetLocation);
}

void AInteractable::SetWidgetVisibility(const ESlateVisibility Visibility) const
{
	if (UUserWidget* WidgetObject = Widget->GetUserWidgetObject())
	{
		WidgetObject->SetVisibility(Visibility);
	}
}

void AInteractable::CompleteProgress()
{
	bIsActive = false;
	UpdateMaterialsOnMesh();
	SetWidgetVisibility(ESlateVisibility::Hidden);
	OnProgressCompleted.Broadcast();
}

void AInteractable::StopProgress()
{
	bSaveProgress
		? PauseProgress()
		: CancelProgress();
}

void AInteractable::UpdateProgress(const float InProgress)
{
	if (!bIsActive)
	{
		return;
	}

	SavedProgress += InProgress;
	if (SavedProgress >= HoldDuration)
	{
		CompleteProgress();

		return;
	}

	UHeistProgressBar* ProgressBar = Cast<UHeistProgressBar>(Widget->GetUserWidgetObject());
	if (ProgressBar != nullptr)
	{
		// Divide the SavedProgress by the HoldDuration to get the progress percentage (a value between 0 and 1)
		ProgressBar->SetProgress(FMath::Clamp(SavedProgress / HoldDuration, 0.f, 1.f));
	}

	SetWidgetVisibility(ESlateVisibility::Visible);
	OnProgressUpdated.Broadcast(SavedProgress);
}

void AInteractable::PauseProgress()
{
	OnProgressPaused.Broadcast();
}

void AInteractable::CancelProgress()
{
	SavedProgress = 0.f;
	SetWidgetVisibility(ESlateVisibility::Hidden);
	OnProgressCancelled.Broadcast();
}
