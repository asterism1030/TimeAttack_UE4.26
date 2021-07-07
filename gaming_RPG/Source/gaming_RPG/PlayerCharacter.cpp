// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Component 생성
	//// Mesh 설정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelObj(TEXT("SkeletalMesh'/Game/Asset/Player/Greystone/Meshes/Greystone.Greystone'"));

	if (SkelObj.Succeeded()) {
		SetSkeletalMesh(SkelObj.Object);
	}
	//// 본 캐릭터가 90도 회전 및 CapsuleCollider에 맞춰야하므로
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));

	//// 콜리전 (무기)
	weaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	weaponCollision->SetupAttachment(GetMesh());
	weaponCollision->AttachTo(GetMesh(), TEXT("FX_Sword_Bottom")); // 소켓
	weaponCollision->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 45.0f), FRotator(0.0f, 0.0f, 90.0f));
	weaponCollision->SetBoxExtent(FVector(5.0f, 70.0f, 7.0f));
	weaponCollision->SetNotifyRigidBodyCollision(true);
	weaponCollision->SetGenerateOverlapEvents(true);
	weaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//// 메인 카메라 스프링 암 생성 및 값 설정
	mainCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCamera_SpringArm"));
	mainCameraSpringArm->SetupAttachment(RootComponent);
	////// 피치를 -60도 (아래로 향하도록), 기본 위치에서 위로 50
	mainCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, -10.0f, 100.0f), FRotator(-40.0f, 0.0f, 0.0f));
	mainCameraSpringArm->TargetArmLength = 300.f;

	mainCameraSpringArm->bUsePawnControlRotation = true;
	mainCameraSpringArm->bInheritPitch = true;
	mainCameraSpringArm->bInheritYaw = true;
	mainCameraSpringArm->bInheritRoll = true;
	//// 메인 카메라 생성 및 설정
	mainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mainCamera->SetupAttachment(mainCameraSpringArm, USpringArmComponent::SocketName);

	//// ai (몹에게 인식가능하도록)
	aiPerceptionStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuli"));
	aiPerceptionStimuli->RegisterComponent();

	FGenericTeamId(0);

	//// Anim Class 설정 (BluePrint 가져옴)
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/BP/Player/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter'"));

	////// Animation 작동하려면 필수!
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	if (AnimBP.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(AnimBP.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Combo1(TEXT("AnimMontage'/Game/Asset/Player/Greystone/Animations/Attack_PrimaryA_Montage.Attack_PrimaryA_Montage'"));
	if (Combo1.Succeeded()) {
		combo1Anim = Combo1.Object; //UAnimMontage*
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Combo2(TEXT("AnimMontage'/Game/Asset/Player/Greystone/Animations/Attack_PrimaryB_Montage.Attack_PrimaryB_Montage'"));
	if (Combo2.Succeeded()) {
		combo2Anim = Combo2.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Combo3(TEXT("AnimMontage'/Game/Asset/Player/Greystone/Animations/Attack_PrimaryC_Montage.Attack_PrimaryC_Montage'"));
	if(Combo3.Succeeded()) {
		combo3Anim = Combo3.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Deflect(TEXT("AnimMontage'/Game/Asset/Player/Greystone/Animations/Deflect_InPlace_Montage.Deflect_InPlace_Montage'"));
	if (Deflect.Succeeded()) {
		guardAnim = Deflect.Object;
	}

	//// 기타 설정
	GetCharacterMovement()->MaxWalkSpeed = 600;
	////// 카메라 방향으로 회전을 위해
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	// 데미지
	//weaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::WeaponDamage);

	SetCanBeDamaged(true);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (GEngine) {
		// 5 초간 디버그 메시지를 표시합니다. (첫 인수인) -1 "Key" 값은 이 메시지를 업데이트 또는 새로고칠 필요가X
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("***!! Character !!***"));
	}
	*/

}


// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
//// Pawn이 사용자 정의 입력 바인딩을 설정할 수 있게 함(이 메소드의 역할!)
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 처리 바인딩 설정
	PlayerInputComponent->BindAxis("Right_Left", this, &APlayerCharacter::RightLeft);
	PlayerInputComponent->BindAxis("Backward_Forward", this, &APlayerCharacter::BackwardForward);

	// 점프 처리 바인딩 설정
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	// 걷기/달리기 처리 바인딩 설정
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Running);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::Walking);

	// 방향 처리 바인딩 설정
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	// 기타 동작
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &APlayerCharacter::Guard);

}

//// 플레이어 정보
USkeletalMeshComponent* APlayerCharacter::GetPawnMesh() const
{
	// 이후에 1인칭 시점으로 변경하고 싶은 경우 리턴 값 수정할 것
	return GetMesh();
}


///// 이동 처리 (진행 방향으로 캐릭터 회전)
void APlayerCharacter::RightLeft(float val)
{
	float rot_roll = 0.0f;
	float rot_pitch = 0.0f;
	float rot_yaw = 0.0f;

	FRotator rot = GetControlRotation();
	UKismetMathLibrary::BreakRotator(rot, rot_roll, rot_pitch, rot_yaw);

	FRotator input_rot = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rot_yaw);
	FVector input_vec = UKismetMathLibrary::GetRightVector(input_rot);

	AddMovementInput(input_vec, val);
}

void APlayerCharacter::BackwardForward(float val)
{
	float rot_roll = 0.0f;
	float rot_pitch = 0.0f;
	float rot_yaw = 0.0f;

	FRotator rot = GetControlRotation();
	UKismetMathLibrary::BreakRotator(rot, rot_roll, rot_pitch, rot_yaw);

	FRotator input_rot = UKismetMathLibrary::MakeRotator(0.0f, 0.0f, rot_yaw);
	FVector input_vec = UKismetMathLibrary::GetForwardVector(input_rot);

	AddMovementInput(input_vec, val);
}


//// 점프 처리
void APlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	bPressedJump = false;
}


//// 방향 처리 (카메라)
void APlayerCharacter::Turn(float val)
{
	AddControllerYawInput(val);
}


void APlayerCharacter::LookUp(float val)
{
	AddControllerPitchInput(val);
}

//// 걷기 및 달리기 등의 모션
void APlayerCharacter::Walking()
{
	GetCharacterMovement()->MaxWalkSpeed = 300;
}

void APlayerCharacter::Running()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000;
}


void APlayerCharacter::Attack()
{
	if (isAttacking == true) {
		saveAttack = true;
	}
	else {
		isAttacking = true;

		ComboAttackSave();
		
		PlayCombo();
	}

}

void APlayerCharacter::Guard()
{
	if (guardAnim == nullptr) {
		return;
	}

	PlayAnimMontage(guardAnim, meleeSpeed);
}

void APlayerCharacter::SetIsHittingAndClear(bool isHit)
{
	isHitting = isHit;
	damagingThings.Empty();
}


//// 메시
void APlayerCharacter::SetSkeletalMesh(USkeletalMesh* mesh) {

	GetMesh()->SkeletalMesh = mesh;

}

//// 데미지 처리
void APlayerCharacter::WeaponDamage(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || isHitting == false) {
		return;
	}

	FName enemy_name = OtherActor->GetFName();

	if (damagingThings.Contains(enemy_name)) {
		return;
	}

	UGameplayStatics::ApplyDamage(OtherActor, base_attk, GetController(), this, NULL);

	damagingThings.Add(enemy_name);

}

float APlayerCharacter::GetDamage(float Damage, UDamageType* DamageType, AController* EventInstigator, AActor* DamageCauser)
{
	remain_hp -= Damage;

	return remain_hp;
}

void APlayerCharacter::ComboAttackSave()
{
	if (saveAttack == true) {
		saveAttack = false;

		PlayCombo();
	}
}

void APlayerCharacter::ResetCombo()
{
	attackCount = 0;
	saveAttack = false;
	isAttacking = false;
}

void APlayerCharacter::PlayCombo()
{
	switch (attackCount) {
	case 0:
		attackCount = 1;
		if (combo1Anim == nullptr) {
			break;
		}
		PlayAnimMontage(combo1Anim, meleeSpeed);
		break;
	case 1:
		attackCount = 2;
		if (combo2Anim == nullptr) {
			break;
		}
		PlayAnimMontage(combo2Anim, meleeSpeed);
		break;
	case 2:
		attackCount = 3;
		if (combo3Anim == nullptr) {
			break;
		}
		PlayAnimMontage(combo3Anim, meleeSpeed);
		break;
	}
}

void APlayerCharacter::UpdateMobKillCnt(int addCnt)
{
	mob_kill_cnt += addCnt;
}




