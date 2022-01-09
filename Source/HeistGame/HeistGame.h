// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHeistGame, Log, All);

constexpr ECollisionChannel Projectile_TraceChannel = ECC_GameTraceChannel2;
constexpr EObjectTypeQuery Interactable_ObjectType = ObjectTypeQuery7;
const FName Interaction_ProfileName = FName(TEXT("Interaction"));