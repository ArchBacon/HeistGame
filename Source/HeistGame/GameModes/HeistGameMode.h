// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HeistGame/UI/Widgets/ScoreWidget.h"
#include "HeistGameMode.generated.h"

UCLASS()
class HEISTGAME_API AHeistGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<UUserWidget> ScoreWidgetClass;

	UPROPERTY()
	UScoreWidget* ScoreWidget;

	int32 Score;

public:
	AHeistGameMode();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AddScore(int32 InScore);
};
