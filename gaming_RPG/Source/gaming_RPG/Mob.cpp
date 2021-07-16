// Fill out your copyright notice in the Description page of Project Settings.


#include "Mob.h"
#include "AIMonster_Controller.h"
#include "Perception/AIPerceptionComponent.h"


// Sets default values
AMob::AMob() {
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Full_HP = 30.0f;
	HittingDamage = 10.0f;

	Event_Reset();
}


// Called when the game starts or when spawned
void AMob::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AMob::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMob::SetMobManager(AMob_Manager* manager)
{
	if(manager != MobManager) {
		MobManager = manager;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ SetMobManager _ Error !"));
	}
}


AMob_Manager* AMob::GetMobManager()
{
	return MobManager;
}

void AMob::SetOriginLocation(FVector location)
{
	this->OriginLocation = location;
}


float AMob::GetFullHP()
{
	return Full_HP;
}

void AMob::SetFullHP(float hp)
{
	if (Full_HP < 0.0f) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ setFullHP _ hp is < 0.0f !"));
		return;
	}

	Full_HP = hp;
}


float AMob::GetRemain_HP()
{
	return Remain_HP;
}


void AMob::SetRemain_HP(float hp)
{
	Remain_HP = hp;
}

AActor* AMob::GetPerceivedActor()
{
	return PerceivedActor;
}

void AMob::SetPerceivedActor(AActor* actor)
{
	PerceivedActor = actor;
}

FTimerHandle AMob::GetTargetLostTimer()
{
	return TargetLostTimer;
}

void AMob::SetTargetLostTimer(FTimerHandle targetLostTimer)
{
	TargetLostTimer = targetLostTimer;
}

void AMob::DetectTarget(AActor* TargetActor)
{
	SetPerceivedActor(TargetActor);

	AAIMonster_Controller* aiCon = Cast<AAIMonster_Controller>(Controller);
	
	if (aiCon == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ DetectTarget _ AIC Error !"));
		return;
	}

	aiCon->UpdateTargetActorKey(TargetActor);
	aiCon->UpdateHasLineOfSightKey(true);
}

void AMob::LostTarget()
{
	SetPerceivedActor(nullptr);

	AAIMonster_Controller* aiCon = Cast<AAIMonster_Controller>(Controller);

	if (aiCon == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ DetectTarget _ AIC Error !"));
		return;
	}

	aiCon->UpdateTargetActorKey(nullptr);
	aiCon->UpdateHasLineOfSightKey(false);
}

bool AMob::GetIsHitting()
{
	return isHitting;
}

void AMob::SetIsHitting(bool isHit)
{
	this->isHitting = isHit;


	if (this->isHitting == false) {
		damagingThings.Empty();
	}

}

void AMob::SetIsHittingAndClear(bool isHit)
{
	this->isHitting = isHit;

	damagingThings.Empty();
}

void AMob::HitAnyDamage(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) {
		return;
	}

	if (isHitting == false) {
		return;
	}

	if (OtherActor != PerceivedActor) { //인지 대상만 공격
		return;
	}
	

	if (Remain_HP <= 0.0f) {
		SetIsHitting(false);
		Event_Inactive();

		return;
	}

	if (OtherActor->IsHidden()) {
		SetPerceivedActor(nullptr);
		return;
	}
	

	FName enemy_name = OtherActor->GetFName();

	if (damagingThings.Contains(enemy_name)) { // 해당 공격을 이미 받은 대상은 제외
		return;
	}


	UGameplayStatics::ApplyDamage(OtherActor, HittingDamage, GetController(), this, NULL);

	damagingThings.Add(enemy_name);
}

float AMob::GetDamage(float damage)
{
	Remain_HP -= damage;

	return Remain_HP;
}

void AMob::SetWhenDamagedSkin(int index, float delayTime)
{
	//TMap<int, UMaterialInterface*> damagedSkin;
	
	UMaterialInterface** material = damagedSkin.Find(index);

	if (material == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ SetWhenDamagedSkin _ material Error !"));
		return;
	}
	
	if (GetMesh() == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ SetWhenDamagedSkin _ GetMesh Error !"));
		return;
	}
	
	if (GetMesh()->GetMaterial(index) == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("AMob.cpp _ SetWhenDamagedSkin _ GetMaterial Error !"));
		return;
	}
	
	originSkin.Add(index, GetMesh()->GetMaterial(index));
	GetMesh()->SetMaterial(index, *material);

	FTimerDelegate timerdelegate = FTimerDelegate::CreateUObject(this, &AMob::SetOriginSkin, index);
	GetWorld()->GetTimerManager().SetTimer(mobWaitHandle, timerdelegate, delayTime, true);

}

void AMob::SetOriginSkin(int index)
{
	GetMesh()->SetMaterial(index, originSkin[index]);
	GetWorld()->GetTimerManager().ClearTimer(mobWaitHandle);
}




void AMob::Event_Inactive()
{

	MobManager->InactiveMob(this);

}

void AMob::Event_Reset()
{
	Remain_HP = Full_HP;

	PerceivedActor = nullptr;
	TargetLostTimer.Invalidate();

	SetIsHitting(false);
	SetCanBeDamaged(true);


}
