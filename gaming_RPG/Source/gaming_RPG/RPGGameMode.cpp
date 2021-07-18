// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGGameMode.h"
#include "gaming_RPG.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.h"

ARPGGameMode::ARPGGameMode() {
	DefaultPawnClass = APlayerCharacter::StaticClass();

	PlayerControllerClass = APlayerCharacterController::StaticClass();
}

void ARPGGameMode::StartPlay() {
	
	Super::StartPlay();

}