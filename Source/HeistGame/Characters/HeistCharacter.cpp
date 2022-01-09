// Fill out your copyright notice in the Description page of Project Settings.

#include "HeistCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeistGame/Controllers/HeistPlayerController.h"
#include "Kismet/GameplayStatics.h"

AHeistCharacter::AHeistCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	SpringArmComponent->TargetArmLength = 1200.f;
	SpringArmComponent->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(SpringArmComponent);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetGenerateOverlapEvents(true);

	Bag = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bag"));
	Bag->SetRelativeLocation(FVector(-4.f, -23.f, 130));
	Bag->SetRelativeRotation(FRotator(24.f, 30.f, -79.f));
	Bag->SetVisibility(false);

	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	bInHeistMode = false;
}

void AHeistCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

void AHeistCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AHeistCharacter::OnFire);
	PlayerInputComponent->BindAction("ActivateHeistMode", IE_Pressed, this, &AHeistCharacter::ActivateHeistMode);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AHeistCharacter::SetMaxWalkSpeedToSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AHeistCharacter::SetMaxWalkSpeedToWalk);
	PlayerInputComponent->BindAction("DropBag", IE_Pressed, this, &AHeistCharacter::DropBag);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeistCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeistCharacter::MoveRight);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHeistCharacter::OnFire()
{
	if (!bInHeistMode)
	{
		return;
	}

	constexpr float ProjectileRange = 1200.f;
	const FVector TraceStart = Weapon->GetSocketLocation("MuzzleFlash") + (GetCapsuleComponent()->GetForwardVector() * 1);
	const FVector TraceEnd = TraceStart + (GetCapsuleComponent()->GetForwardVector() * ProjectileRange);

	// TODO: Replace Debug with ACTUAL Particle
	// TODO: Add laser pointer

	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		TraceStart,
		TraceEnd,
		TraceTypeQuery3, // Projectile
		true,
		{},
		EDrawDebugTrace::ForDuration,
		OutHit,
		true,
		FColor::Black,
		FColor::Green,
		0.4f
	);

	if (OutHit.bBlockingHit)
	{
		constexpr float WeaponDamage = 10.f;
		UGameplayStatics::ApplyPointDamage(
			OutHit.GetActor(),
			WeaponDamage,
			GetActorLocation(),
			OutHit,
			GetController(),
			this,
			UDamageType::StaticClass()
		);
	}

	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AHeistCharacter::MoveForward(const float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AHeistCharacter::MoveRight(const float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AHeistCharacter::ActivateHeistMode()
{
	if (!bInHeistMode)
	{
		bInHeistMode = true;
		GetMesh()->SetAnimation(WeaponDrawnAnim);

		AHeistPlayerController* HeistController = Cast<AHeistPlayerController>(GetController());
		HeistController->ActivateHeistMode();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHeistCharacter::SetMaxWalkSpeedToSprint()
{
	if (bInHeistMode && !Bag->IsVisible())
	{
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHeistCharacter::SetMaxWalkSpeedToWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AHeistCharacter::DropBag()
{
	if (bInHeistMode && Bag->IsVisible())
	{
		FVector Location = GetActorLocation();
		Location.Z = 110.f;

		FRotator Rotation = GetMesh()->GetComponentRotation();
		Rotation.Yaw += 90.f;

		ADuffleBag* SpawnedBag = GetWorld()->SpawnActor<ADuffleBag>(
			ADuffleBag::StaticClass(),
			Location,
			Rotation
		);

		SpawnedBag->SetContents(BagContents);
		Bag->SetVisibility(false);
	}
}
