// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Mob.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Mob_Manager.generated.h"

/*
* - ���� �Ŵ��� -
* 1. Ư�� �ð��� �귶�� �� ������ ���ڿ� ���缭 ���� ����
*    ������ ��ġ�� ���� ���� ���� ��ġ�� ��
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
	//// ������Ƽ
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