// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

UCLASS()
class HEISTGAME_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	int32 Score;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnScoreUpdate(int32 InScore);
};
