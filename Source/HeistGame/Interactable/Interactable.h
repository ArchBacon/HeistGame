// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

class UWidgetComponent;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProgressCompletedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProgressPausedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FProgressCancelledSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMaterialsUpdatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProgressUpdatedSignature, float, NewProgress);

UCLASS()
class HEISTGAME_API AInteractable : public AActor
{
	GENERATED_BODY()

	UPROPERTY(Category=Components, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(Category=Components, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(Category=Components, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	UWidgetComponent* Widget;

	UPROPERTY(Category=Materials, EditDefaultsOnly)
	UMaterialInterface* InactiveMaterial;

	UPROPERTY(Category=Materials, EditDefaultsOnly)
	UMaterialInterface* ActiveMaterial;

	UPROPERTY(Category=Config, EditAnywhere)
	bool bIsActive;

	UPROPERTY(Category=Config, EditDefaultsOnly)
	bool bSaveProgress;

	UPROPERTY(Category=Config, EditDefaultsOnly)
	float HoldDuration;

	UPROPERTY()
	float SavedProgress;
	
public:
	AInteractable();

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable)
	FProgressCompletedSignature OnProgressCompleted;

	UPROPERTY(BlueprintAssignable)
	FProgressPausedSignature OnProgressPaused;

	UPROPERTY(BlueprintAssignable)
	FProgressCancelledSignature OnProgressCancelled;

	UPROPERTY(BlueprintAssignable)
	FProgressUpdatedSignature OnProgressUpdated;

	UPROPERTY(BlueprintAssignable)
	FMaterialsUpdatedSignature OnMaterialsUpdated;

protected:
	UFUNCTION(BlueprintCallable)
	void UpdateMaterialsOnMesh() const;

	UFUNCTION(BlueprintCallable)
	void UpdateWidgetLocation() const;

	UFUNCTION()
	void SetWidgetVisibility(ESlateVisibility Visibility) const;

public:
	UFUNCTION(BlueprintCallable)
	virtual void CompleteProgress();

	UFUNCTION(BlueprintCallable)
	virtual void StopProgress();

	UFUNCTION(BlueprintCallable)
	virtual void UpdateProgress(float InProgress);

protected:
	UFUNCTION()
	virtual void PauseProgress();

	UFUNCTION()
	virtual void CancelProgress();

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsActive() const { return bIsActive; }

	UFUNCTION(BlueprintCallable)
	void SetActive(bool IsActive) { bIsActive = IsActive; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetSavedProgress() const { return SavedProgress; }
};
