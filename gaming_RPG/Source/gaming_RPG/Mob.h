// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mob_Manager.h"

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "Mob.generated.h"


UCLASS()
class GAMING_RPG_API AMob : public ACharacter
{
	GENERATED_BODY()

public:
	AMob();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	UFUNCTION(BlueprintCallable, Category = "Mob_Event")
		void Event_Inactive();

	UFUNCTION(BlueprintCallable, Category = "Mob_Event")
		void Event_Reset();

	float GetFullHP();

	void SetFullHP(float hp);

	float GetRemain_HP();

	void SetRemain_HP(float hp);

	AActor* GetPerceivedActor();

	void SetPerceivedActor(AActor* actor);

	FTimerHandle GetTargetLostTimer();

	void SetTargetLostTimer(FTimerHandle targetLostTimer);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		void DetectTarget(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		void LostTarget();

	bool GetIsHitting();

	void SetIsHitting(bool isHit);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		void SetIsHittingAndClear(bool isHit);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		virtual void HitAnyDamage(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		virtual float GetDamage(float damage);

	UFUNCTION(BlueprintCallable, Category = "Mob_State")
		virtual void SetWhenDamagedSkin(int index, float delayTime);

	virtual void SetOriginSkin(int index);

	UFUNCTION()
		void SetMobManager(AMob_Manager* manager);
	
	UFUNCTION()
		AMob_Manager* GetMobManager();

	void SetOriginLocation(FVector location);

private:
	FTimerHandle mobWaitHandle;

	//프로퍼티
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Anim", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DeathAnim;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Ect", meta = (AllowPrivateAccess = "true"))
		TSet<FName> damagingThings; //중복 공격 방지

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Ect", meta = (AllowPrivateAccess = "true"))
		TMap<int, UMaterialInterface*> damagedSkin;

	TMap<int, UMaterialInterface*> originSkin;

	UPROPERTY(VisibleAnywhere, Category = "Mob_Ect", meta = (AllowPrivateAccess = "true"))
		AMob_Manager* MobManager;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Info", meta = (AllowPrivateAccess = "true"))
		float Full_HP;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Info", meta = (AllowPrivateAccess = "true"))
		float Remain_HP;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_Info", meta = (AllowPrivateAccess = "true"))
		float HittingDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_AI", meta = (AllowPrivateAccess = "true"))
		FVector OriginLocation;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_AI", meta = (AllowPrivateAccess = "true"))
		AActor* PerceivedActor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mob_AI", meta = (AllowPrivateAccess = "true"))
		FTimerHandle TargetLostTimer;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Mob_State", meta = (AllowPrivateAccess = "true"))
		bool isHitting;
	
};