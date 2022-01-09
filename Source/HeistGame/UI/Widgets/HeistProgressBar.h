// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeistProgressBar.generated.h"

UCLASS()
class HEISTGAME_API UHeistProgressBar : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	float Progress;

public:
	UFUNCTION(BlueprintNativeEvent)
	void SetProgress(float Inprogress);
};
