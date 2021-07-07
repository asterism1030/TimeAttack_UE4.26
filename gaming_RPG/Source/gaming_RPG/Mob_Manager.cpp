// Fill out your copyright notice in the Description page of Project Settings.


#include "Mob_Manager.h"
#include "AIController.h"

// Sets default values
AMob_Manager::AMob_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	static ConstructorHelpers::FClassFinder<ACharacter> BP_Mob(TEXT("/Game/BP/Mob/BP_Mob"));
	if (BP_Mob.Succeeded()) {
		base_mob = BP_Mob.Class;
	}
	*/
	
}

// Called when the game starts or when spawned
void AMob_Manager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMob();

}

// Called every frame
void AMob_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMob_Manager::SpawnMob()
{
	UE_LOG(LogTemp, Log, TEXT("AMob_Manager.cpp _ spawnMob _ Called !"));

	if (base_mob == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob_Manager.cpp _ spawnMob _ base_mob is null !"));
		return;
	}

	int remainCnt = mobs_All.Num();
	
	// 최초 spawn 시
	while (remainCnt < spawnCnt) {

		if (!GetWorld()) {
			UE_LOG(LogTemp, Error, TEXT("AMob_Manager.cpp _ spawnMob _ GetWorld is null !"));
			return;
		}

		AMob* mobSpawned = GetWorld()->SpawnActor<AMob>((UClass*)base_mob, GetTransform().GetLocation(), 
			FRotator(GetTransform().GetRotation()));
		
		mobSpawned->SetMobManager(this);
		mobSpawned->SetOriginLocation(GetTransform().GetLocation());
		mobs_All.Add(mobSpawned);

		remainCnt++;
	}

	// Inactive 후 조건에 의해 불러질 시
	if(mobs_UnActive.Num() != 0) {
		for (auto mob : mobs_UnActive) {
			mob->SetActorTickEnabled(true);
			mob->SetActorHiddenInGame(false);
			mob->SetActorEnableCollision(true);

			mob->Event_Reset();
		}

		mobs_UnActive.Empty();

		GetWorld()->GetTimerManager().ClearTimer(waitHandle);
	}
}

void AMob_Manager::InactiveMob(AMob* mob)
{
	UE_LOG(LogTemp, Log, TEXT("AMob_Manager.cpp _ InactiveMob _ Called !"));

	if (!mobs_UnActive.Contains(mob)) {
		mob->SetActorTickEnabled(false);
		mob->SetActorHiddenInGame(true);
		mob->SetActorEnableCollision(false);

		mobs_UnActive.Add(mob);
	}

	if (mobs_UnActive.Num() - spawnCnt <= 1) {
		GetWorld()->GetTimerManager().SetTimer(waitHandle, this, &AMob_Manager::SpawnMob, spawnMeleeTime, true);
	}
}



