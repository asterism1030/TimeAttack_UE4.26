// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Component ����
	//// Mesh ����
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelObj(TEXT("SkeletalMesh'/Game/Asset/Player/Greystone/Meshes/Greystone.Greystone'"));

	if (SkelObj.Succeeded()) {
		SetSkeletalMesh(SkelObj.Object);
	}
	//// �� ĳ���Ͱ� 90�� ȸ�� �� CapsuleCollider�� ������ϹǷ�
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));

	//// �ݸ��� (����)
	weaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	weaponCollision->SetupAttachment(GetMesh());
	weaponCollision->AttachTo(GetMesh(), TEXT("FX_Sword_Bottom")); // ����
	weaponCollision->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 45.0f), FRotator(0.0f, 0.0f, 90.0f));
	weaponCollision->SetBoxExtent(FVector(5.0f, 70.0f, 7.0f));
	weaponCollision->SetNotifyRigidBodyCollision(true);
	weaponCollision->SetGenerateOverlapEvents(true);
	weaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	//// ���� ī�޶� ������ �� ���� �� �� ����
	mainCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainCamera_SpringArm"));
	mainCameraSpringArm->SetupAttachment(RootComponent);
	////// ��ġ�� -60�� (�Ʒ��� ���ϵ���), �⺻ ��ġ���� ���� 50
	mainCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, -10.0f, 100.0f), FRotator(-40.0f, 0.0f, 0.0f));
	mainCameraSpringArm->TargetArmLength = 300.f;

	mainCameraSpringArm->bUsePawnControlRotation = true;
	mainCameraSpringArm->bInheritPitch = true;
	mainCameraSpringArm->bInheritYaw = true;
	mainCameraSpringArm->bInheritRoll = true;
	//// ���� ī�޶� ���� �� ����
	mainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	mainCamera->SetupAttachment(mainCameraSpringArm, USpringArmComponent::SocketName);

	//// ai (������ �νİ����ϵ���)
	aiPerceptionStimuli = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuli"));
	aiPerceptionStimuli->RegisterComponent();

	FGenericTeamId(0);

	//// Anim Class ���� (BluePrint ������)
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP(TEXT("AnimBlueprint'/Game/BP/Player/AnimBP_PlayerCharacter.AnimBP_PlayerCharacter'"));

	////// Animation �۵��Ϸ��� �ʼ�!
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

	//// ��Ÿ ����
	GetCharacterMovement()->MaxWalkSpeed = 600;
	////// ī�޶� �������� ȸ���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;


	// ������
	//weaponCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::WeaponDamage);

	SetCanBeDamaged(true);

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*
	if (GEngine) {
		// 5 �ʰ� ����� �޽����� ǥ���մϴ�. (ù �μ���) -1 "Key" ���� �� �޽����� ������Ʈ �Ǵ� ���ΰ�ĥ �ʿ䰡X
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
//// Pawn�� ����� ���� �Է� ���ε��� ������ �� �ְ� ��(�� �޼ҵ��� ����!)
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �̵� ó�� ���ε� ����
	PlayerInputComponent->BindAxis("Right_Left", this, &APlayerCharacter::RightLeft);
	PlayerInputComponent->BindAxis("Backward_Forward", this, &APlayerCharacter::BackwardForward);

	// ���� ó�� ���ε� ����
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJump);

	// �ȱ�/�޸��� ó�� ���ε� ����
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Running);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::Walking);

	// ���� ó�� ���ε� ����
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);

	// ��Ÿ ����
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this, &APlayerCharacter::Guard);

}

//// �÷��̾� ����
USkeletalMeshComponent* APlayerCharacter::GetPawnMesh() const
{
	// ���Ŀ� 1��Ī �������� �����ϰ� ���� ��� ���� �� ������ ��
	return GetMesh();
}


///// �̵� ó�� (���� �������� ĳ���� ȸ��)
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


//// ���� ó��
void APlayerCharacter::StartJump()
{
	bPressedJump = true;
}

void APlayerCharacter::StopJump()
{
	bPressedJump = false;
}


//// ���� ó�� (ī�޶�)
void APlayerCharacter::Turn(float val)
{
	AddControllerYawInput(val);
}


void APlayerCharacter::LookUp(float val)
{
	AddControllerPitchInput(val);
}

//// �ȱ� �� �޸��� ���� ���
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


//// �޽�
void APlayerCharacter::SetSkeletalMesh(USkeletalMesh* mesh) {

	GetMesh()->SkeletalMesh = mesh;

}

//// ������ ó��
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




