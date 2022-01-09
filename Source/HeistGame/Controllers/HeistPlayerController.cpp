// Fill out your copyright notice in the Description page of Project Settings.

#include "HeistPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/InputSettings.h"
#include "HeistGame/HeistGame.h"
#include "Kismet/KismetSystemLibrary.h"

AHeistPlayerController::AHeistPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AHeistPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AHeistPlayerController::StartInteractKeyDown);
	InputComponent->BindAction("Interact", IE_Released, this, &AHeistPlayerController::StopInteractKeyDown);
}

void AHeistPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RotateCharacterToCursor();

	if (bInHeistMode && bIsInteractKeyDown)
	{
		AInteractable* FoundInteractable = TryFindInteractable();
		if (IsValid(FoundInteractable) && FoundInteractable->IsActive())
		{
			InteractableInRange = FoundInteractable;
			InteractableInRange->UpdateProgress(DeltaSeconds);
		}

		return;
	}

	if (IsValid(InteractableInRange))
	{
		InteractableInRange->StopProgress();
		InteractableInRange = nullptr;
	}
}

void AHeistPlayerController::RotateCharacterToCursor()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, CursorHitResult);
	
	GetCharacter()->SetActorRotation(FindLookAtRotation(
		GetCharacter()->GetActorLocation(),
		CursorHitResult.Location
	));
}

FRotator AHeistPlayerController::FindLookAtRotation(const FVector Start, const FVector Target)
{
	FVector Direction = Target - Start;
	Direction.Z = 0;

	return FRotationMatrix::MakeFromX(Direction).Rotator();
}

AInteractable* AHeistPlayerController::TryFindInteractable() const
{
	constexpr float InteractionRange = 120.f;
	const FVector TraceStart = GetCharacter()->GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetCharacter()->GetActorForwardVector() * InteractionRange);

	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		TraceStart,
		TraceEnd,
		{Interactable_ObjectType},
		true,
		{}, // TODO: Ignore All interactables that are inactive
		EDrawDebugTrace::ForOneFrame,
		OutHit,
		true,
		FColor::Red,
		FColor::Green,
		0.f
	);

	if (OutHit.bBlockingHit)
	{
		return Cast<AInteractable>(OutHit.Actor);
	}

	return nullptr;
}

void AHeistPlayerController::StartInteractKeyDown()
{
	bIsInteractKeyDown = true;
}

void AHeistPlayerController::StopInteractKeyDown()
{
	bIsInteractKeyDown = false;
}

