// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "GenericTeamAgentInterface.h"

#include "AIMonster_Controller.generated.h"

/**
 * 
 */
UCLASS()
class GAMING_RPG_API AAIMonster_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AAIMonster_Controller();

	//ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable, Category = "AIC_Func")
		void UpdateTargetActorKey(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "AIC_Func")
		void UpdateHasLineOfSightKey(bool HasLineOfSight);

private:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIC_Key", meta = (AllowPrivateAccess = "true"))
		FName TargetActorName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AIC_Key", meta = (AllowPrivateAccess = "true"))
		FName HasLineOfSightName;

};
