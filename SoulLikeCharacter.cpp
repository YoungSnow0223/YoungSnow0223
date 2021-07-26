// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeCharacter.h"
#include "SoulLike.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "Engine/TargetPoint.h"
#include "SoulLikeGameMode.h"

// Sets default values
ASoulLikeCharacter::ASoulLikeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ī�޶� rotation ���
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	

	// �ݸ��� ĸ�� ũ�� ����
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint> myAnimBP(TEXT("/Game/PlayerCharacter/Animation/SLG_Character_AnimBP.SLG_Character_AnimBP"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> myMesh(TEXT("/Game/UE4/Content/Barbarian/Mesh/SK_Barbarian_Full.SK_Barbarian_Full"));
	// load AnimMontage
	ConstructorHelpers::FObjectFinder<UAnimMontage> myAnimMontage1(TEXT("/Game/PlayerCharacter/Animation/CharacterAttack1Montage.CharacterAttack1Montage"));

	if (myMesh.Succeeded() && myAnimBP.Succeeded() && myAnimMontage1.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object->GeneratedClass);
		CharacterAttack1Montage = myAnimMontage1.Object;
	}

	// �޽ø� ȸ���ϰ� ������ ĸ�� ������Ʈ�� �˸°� �����
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// ĳ���� �̵� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f;

	// ī�޶� �� ����� (�ݸ����� �ִٸ� �÷��̾� ������ �������)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom);

	CameraBoom->AttachTo(RootComponent);

	// ī�޶� ĳ���� ���� ���� ���ݸ�ŭ ����´�
	CameraBoom->TargetArmLength = 250.0f;

	// rotate arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	// �÷��̾���� ������
	CameraBoom->AddRelativeLocation(FVector(0.0f, 100.0f, 160.0f));

	// Follow ī�޶� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	check(FollowCamera);

	// �� ���� ī�޶� �����ϰ� ���� ��Ʈ�ѷ� ����� ��ġ�ϵ��� �����Ѵ�
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// FollowCamera cannot controll rotation
	FollowCamera->bUsePawnControlRotation = false;
	// ī�޶� �ణ �����ٺ����� �ϱ� ���� ȸ�� ����
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-30.0f, 0.0f, 0.0f)));

	// ID 0(�⺻ ��Ʈ�ѷ�)�� �Է� ��������
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ASoulLikeCharacter::BeginPlay()
{
	Super::BeginPlay();

	/*for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		TargetArray.Add(*TargetIter);*/
	
}

// Called every frame
void ASoulLikeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void ASoulLikeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �����÷��� Ű ���ε� �����ϱ�
	check(PlayerInputComponent);

	// ����
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// ������ �̵�
	PlayerInputComponent->BindAxis("MoveForward", this, &ASoulLikeCharacter::MoveForward);

	// ���������� �̵�
	PlayerInputComponent->BindAxis("MoveRight", this, &ASoulLikeCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// ���̽�ƽ: PlayerInputComponent->BindAxis("TurnRate", this, &ASoulLikeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// ���̽�ƽ PlayerInputComponent->BindAxis("LookUpRate", this, &ASoulLikeCharacter::LookUpAtRate);

	// ���� ���Ľ��� ��ȯ
	/*PlayerInputComponent->BindAction("ArmPlayer", IE_Pressed, this, &ASoulLikeCharacter::ArmPlayer);*/

	// ���� 1
	PlayerInputComponent->BindAction("Attack1", IE_Pressed, this, &ASoulLikeCharacter::Attack1);
	PlayerInputComponent->BindAction("Attack1", IE_Released, this, &ASoulLikeCharacter::StopAttack1);
}

void ASoulLikeCharacter::MoveForward(float Value)
{

	if ((Controller) && (Value != 0.0f) && IsKeyboardEnabled)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASoulLikeCharacter::MoveRight(float Value)
{
	if ((Controller) && (Value != 0.0f) && IsKeyboardEnabled)
	{
		// find out which way is right
		FRotator Rotation = Controller->GetControlRotation();
		FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASoulLikeCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());

}

void ASoulLikeCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}




/*bool ASoulLikeCharacter::IsArmed()
{
	return bIsArmed;
}*/

/*void ASoulLikeCharacter::ArmPlayer()
{
	//flip . flop
	bIsArmed = !bIsArmed;

	if (!bIsArmed)
	{
		CountdownToIdle = MaxCountdownToIdle;
		GetWorld()->GetTimerManager().ClearTimer(ArmedToIdleTimerHandle);
	}
}*/

/*void ASoulLikeCharacter::TriggerCountdownToIdle()
{
	if (--CountdownToIdle <= 0)
	{
		bIsArmed = false;
		CountdownToIdle = MaxCountdownToIdle;
		GetWorld()->GetTimerManager().ClearTimer(ArmedToIdleTimerHandle);
	}
}*/

void ASoulLikeCharacter::SetIsKeyboardEnabled(bool Enabled)
{
	IsKeyboardEnabled = Enabled;
}

void ASoulLikeCharacter::Attack1()
{
	IsKeyboardEnabled = true;

	PlayAnimMontage(CharacterAttack1Montage, 1.0f, FName("start_1"));
	/*if (!bIsArmed)
	{
		bIsArmed = true;
	}*/

	IsKeyboardEnabled = false;

}

void ASoulLikeCharacter::StopAttack1()
{
	IsKeyboardEnabled = true;

	/*bool bIsCountdownActive = GetWorld()->GetTimerManager().IsTimerActive(ArmedToIdleTimerHandle);
	if (bIsCountdownActive)
	{
		// reset timer
		bool bIsaActive = GetWorld()->GetTimerManager().IsTimerActive(ArmedToIdleTimerHandle);
	}*/

	// start the timer again
	/*CountdownToIdle = MaxCountdownToIdle;
	GetWorld()->GetTimerManager().SetTimer(ArmedToIdleTimerHandle, this, &ASoulLikeCharacter::TriggerCountdownToIdle, 1.0f, true);*/
}


