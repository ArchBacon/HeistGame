// Fill out your copyright notice in the Description page of Project Settings.


#include "HeistGameMode.h"

AHeistGameMode::AHeistGameMode()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/HeistGame/UI/WBP_Score"));
	if (WidgetClassFinder.Class != nullptr)
	{
		ScoreWidgetClass = WidgetClassFinder.Class;
	}
}

void AHeistGameMode::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Move to game instance
	ScoreWidget = Cast<UScoreWidget>(CreateWidget(GetWorld(), ScoreWidgetClass));
	ScoreWidget->AddToViewport();
}

void AHeistGameMode::AddScore(int32 InScore)
{
	Score += InScore;
	ScoreWidget->OnScoreUpdate(Score);
}
