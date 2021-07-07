// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


UCLASS(Blueprintable)
class GAMING_RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY() //! 에러가 발생함에도 잘 컴파일 됨 !

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//// 플레이어 정보
	USkeletalMeshComponent* GetPawnMesh() const;

	//// 액션 / 축 맵핑 (기본 동작)
	UFUNCTION()
		void RightLeft(float val);

	UFUNCTION()
		void BackwardForward(float val);

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void StartJump();

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void StopJump();

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void Turn(float val);

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void LookUp(float val);

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void Running();

	UFUNCTION(BlueprintCallable, Category = "User_BasicMotion")
		void Walking();

	//// 기타 동작
	UFUNCTION(BlueprintCallable, Category = "User_Motion")
		void Attack();

	UFUNCTION(BlueprintCallable, Category = "User_Motion")
		void Guard();

	UFUNCTION(BlueprintCallable, Category = "User_Motion")
		void SetIsHittingAndClear(bool isHit);

	//// 메시
	void SetSkeletalMesh(USkeletalMesh* mesh);

	//// 데미지 처리
	UFUNCTION(BlueprintCallable, Category = "User_Damage")
		void WeaponDamage(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "User_Damage")
		virtual float GetDamage(float Damage, UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "User_Attack")
		void ComboAttackSave();

	UFUNCTION(BlueprintCallable, Category = "User_Attack")
		void ResetCombo();

	void PlayCombo();

	UFUNCTION(BlueprintCallable, Category = "User_Info_Update")
		void UpdateMobKillCnt(int addCnt);

private:
	//프로퍼티
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Info", meta = (AllowPrivateAccess = "true"))
		float full_hp = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Info", meta = (AllowPrivateAccess = "true"))
		float remain_hp = 100.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Info", meta = (AllowPrivateAccess = "true"))
		float base_attk = 10.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Info", meta = (AllowPrivateAccess = "true"))
		int mob_kill_cnt = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "User_Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* combo1Anim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "User_Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* combo2Anim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "User_Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* combo3Anim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "User_Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* guardAnim;

	//// 카메라
	//////! Component의 경우 Detail 패널에 안뜲,,!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* mainCameraSpringArm;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* mainCamera;

	//// AI
	UPROPERTY(BlueprintReadWrite, Category = "User_AI", EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UAIPerceptionStimuliSourceComponent* aiPerceptionStimuli;

	//// 이동값
	float curRLval;
	float curUDval;

	//// 공격 및 스킬
	UPROPERTY(BlueprintReadWrite, Category = "User_Attack", EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int combo = 0;

	//// 무기
	UPROPERTY(BlueprintReadWrite, Category = "User_Attack", EditAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* weaponCollision;

	//// 데미지
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		TSet<FName> damagingThings;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		bool isHitting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		float meleeSpeed = 1.2f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		bool isAttacking = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		bool saveAttack = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "User_Attack", meta = (AllowPrivateAccess = "true"))
		int attackCount = 0;

};
