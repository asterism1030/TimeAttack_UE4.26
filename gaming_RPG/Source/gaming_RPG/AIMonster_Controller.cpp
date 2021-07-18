// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMonster_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

AAIMonster_Controller::AAIMonster_Controller() {
	SetGenericTeamId(FGenericTeamId(2));

	TargetActorName = FName("TargetActor");
	HasLineOfSightName = FName("HasLineOfSight");
}

/*
ETeamAttitude::Type AAIMonster_Controller::GetTeamAttitudeTowards(const AActor& Other) const
{
	FString str = Other.GetName();
	int cnt = 0;
	const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Cast<APawn>(&Other)->GetController());
	str += "   ";
	cnt = TeamAgent->GetGenericTeamId().GetId();
	str += FString::FromInt(cnt);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, str);

	if (const APawn* OtherPawn = Cast<APawn>(&Other)) {

		if (const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			return Super::GetTeamAttitudeTowards(*OtherPawn->GetController());
		}
	}

	return ETeamAttitude::Neutral;
}
*/

void AAIMonster_Controller::UpdateTargetActorKey(AActor* TargetActor)
{
	if (Blackboard) {
		Blackboard->SetValueAsObject(TargetActorName, TargetActor);
	}
}

void AAIMonster_Controller::UpdateHasLineOfSightKey(bool HasLineOfSight)
{
	if (Blackboard) {
		Blackboard->SetValueAsBool(HasLineOfSightName, HasLineOfSight);
	}
}
