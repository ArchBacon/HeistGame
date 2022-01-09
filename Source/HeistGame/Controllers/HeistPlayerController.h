// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HeistGame/Interactable/Interactable.h"
#include "HeistPlayerController.generated.h"

UCLASS()
class HEISTGAME_API AHeistPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	AInteractable* InteractableInRange;

	bool bIsInteractKeyDown;

	bool bInHeistMode;

public:
	AHeistPlayerController();

	virtual void SetupInputComponent() override;

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	void RotateCharacterToCursor();
	FRotator FindLookAtRotation(FVector Start, FVector Target);

public:
	AInteractable* TryFindInteractable() const;

	void ActivateHeistMode() { bInHeistMode = true; }

private:
	UFUNCTION()
	void StartInteractKeyDown();

	UFUNCTION()
	void StopInteractKeyDown();
};
