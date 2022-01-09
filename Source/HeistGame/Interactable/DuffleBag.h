// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "DuffleBag.generated.h"

UENUM()
enum EBagContents
{
	ThermalDrill,
	Cash
};

UCLASS()
class HEISTGAME_API ADuffleBag : public AInteractable
{
	GENERATED_BODY()

	EBagContents BagContents;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EBagContents GetContents() const { return BagContents; }
	
	UFUNCTION(BlueprintCallable)
	void SetContents(EBagContents InContents) { BagContents = InContents; }
};
