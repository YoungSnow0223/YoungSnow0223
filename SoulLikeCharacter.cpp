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

	// 카메라가 rotation 담당
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	

	// 콜리전 캡슐 크기 조절
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

	// 메시를 회전하고 움직여 캡슐 컴포넌트에 알맞게 맞춘다
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));

	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// 캐릭터 이동 구성
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 350.0f;

	// 카메라 붐 만들기 (콜리전이 있다면 플레이어 쪽으로 끌어당긴다)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	check(CameraBoom);

	CameraBoom->AttachTo(RootComponent);

	// 카메라가 캐릭터 뒤의 일정 간격만큼 따라온다
	CameraBoom->TargetArmLength = 250.0f;

	// rotate arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	// 플레이어로의 오프셋
	CameraBoom->AddRelativeLocation(FVector(0.0f, 100.0f, 160.0f));

	// Follow 카메라 생성
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	check(FollowCamera);

	// 붐 끝에 카메라를 연결하고 붐이 컨트롤러 방향과 일치하도록 조정한다
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	// FollowCamera cannot controll rotation
	FollowCamera->bUsePawnControlRotation = false;
	// 카메라가 약간 내려다보도록 하기 위한 회전 변경
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-30.0f, 0.0f, 0.0f)));

	// ID 0(기본 컨트롤러)의 입력 가져오기
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

	// 게임플레이 키 바인드 설정하기
	check(PlayerInputComponent);

	// 구르기
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACharacter::Dodge);
	PlayerInputComponent->BindAction("Dodge", IE_Released, this, &ACharacter::Dodging);

	// 앞으로 이동
	PlayerInputComponent->BindAxis("MoveForward", this, &ASoulLikeCharacter::MoveForward);

	// 오른쪽으로 이동
	PlayerInputComponent->BindAxis("MoveRight", this, &ASoulLikeCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	// 조이스틱: PlayerInputComponent->BindAxis("TurnRate", this, &ASoulLikeCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	// 조이스틱 PlayerInputComponent->BindAxis("LookUpRate", this, &ASoulLikeCharacter::LookUpAtRate);

	// 공격 스탠스로 전환
	/*PlayerInputComponent->BindAction("ArmPlayer", IE_Pressed, this, &ASoulLikeCharacter::ArmPlayer);*/

	// 공격 1
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


