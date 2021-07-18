// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class GAMING_RPG_API APlayerCharacterController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacterController();
	
	FGenericTeamId TeamId;
	FGenericTeamId GetGenericTeamId() const;
};
