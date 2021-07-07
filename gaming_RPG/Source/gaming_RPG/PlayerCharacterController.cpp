// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterController.h"

APlayerCharacterController::APlayerCharacterController() {
	PrimaryActorTick.bCanEverTick = true;
	TeamId = FGenericTeamId(1);
}

FGenericTeamId APlayerCharacterController::GetGenericTeamId() const
{
	return TeamId;
}