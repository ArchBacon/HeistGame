// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeistGame/Interactable/DuffleBag.h"
#include "HeistGame/Interactable/Interactable.h"
#include "HeistCharacter.generated.h"

class AHeistProjectile;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class HEISTGAME_API AHeistCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	UStaticMeshComponent* Bag;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category=Mesh, meta=(AllowPrivateAccess="true"))
	bool bInHeistMode;

	UPROPERTY(EditDefaultsOnly)
	UAnimationAsset* WeaponDrawnAnim;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category=Mesh, meta=(AllowPrivateAccess="true"))
	TEnumAsByte<EBagContents> BagContents;

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	UAnimMontage* FireAnimation;

	AHeistCharacter();

	void OnFire();
	void MoveForward(float Value);
	void MoveRight(float Value);

	FORCEINLINE USkeletalMeshComponent* GetWeapon() const { return Weapon; }

	void ActivateHeistMode();

private:
	UFUNCTION()
	void SetMaxWalkSpeedToSprint();

	UFUNCTION()
	void SetMaxWalkSpeedToWalk();

	UFUNCTION()
	void DropBag();
};
