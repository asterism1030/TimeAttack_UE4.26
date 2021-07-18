// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Mob.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mob_Manager.generated.h"

/*
* - 몬스터 매니저 -
* 1. 특정 시간이 흘렀을 때 지정한 숫자에 맞춰서 몬스터 스폰
*    스폰할 위치에 지면 보다 높게 배치할 것
*/
UCLASS()
class GAMING_RPG_API AMob_Manager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMob_Manager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "MM_Func")
		void SpawnMob();

	UFUNCTION(BlueprintCallable, Category = "MM_Func")
		void InactiveMob(AMob* mob);


private:
	//// 프로퍼티
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MM_Spawn", meta = (AllowPrivateAccess = "true"))
		float spawnMeleeTime = 10.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MM_Spawn", meta = (AllowPrivateAccess = "true"))
		int spawnCnt = 3;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MM_Spawn", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ACharacter> base_mob;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MM_Info", meta = (AllowPrivateAccess = "true"))
		TSet<AMob*> mobs_All;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MM_Info", meta = (AllowPrivateAccess = "true"))
		TSet<AMob*> mobs_UnActive;
		
	FTimerHandle waitHandle;
};