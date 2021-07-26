// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFrameWork/Controller.h"
#include "GameFrameWork/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "SoulLikeCharacter.generated.h"




UCLASS()
class SOULLIKE_API ASoulLikeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// 이동 방향 배열
	UPROPERTY(EditAnywhere, Category = Logic) TArray<class ATargetPoint*> TargetArray;
	// 걷는 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* walkingSound;
	// 뛰는 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* runningSound;
	// 타격 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* hitEnemySound;
	// 맞는 사운드를 위한 오디오 컴포넌트
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* damageSound;

protected:
	// 플레이어와 카메라 간의 거리를 유지하기 위한 카메라 붐
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class USpringArmComponent* CameraBoom;

	// 타깃 뷰로 사용할 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class UCameraComponent* FollowCamera;

	// Character Attack1 montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* CharacterAttack1Montage;

public:

	// 키보드 입력조절
	UFUNCTION(BlueprintCallable, Category = "Animation")
		void SetIsKeyboardEnabled(bool Enabled);

	// 플레이어 앞뒤, 좌우의 타깃 위치 이동을 조정한다
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") float BaseLookUpRate;
	

	/*UFUNCTION(BlueprintCallable, Category = "Animation")
		bool IsArmed();
		*/

	// 플레이어가 공격자세로 스텐스 변경
	/*UFUNCTION()
		void ArmPlayer();
		*/

	// 특정 시간 이후 idle 스텐스로 변경
	UFUNCTION()
		void TriggerCountdownToIdle();

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
		int32 MaxCountdownToIdle;
		*/



	// 플레이어가 공격 1 시작
	void Attack1();
	// 플레이어가 공격 1 중단
	void StopAttack1();


	/* disable controller input
	void DisableInput(APlayerController* PlayerController);
	*/
public:
	// Sets default values for this character's properties
	ASoulLikeCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 캐릭터 선 위치 데이터
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;

	//bool bIsArmed;
	//FTimerHandle ArmedToIdleTimerHandle;
	//int32 CountdownToIdle;

	bool IsKeyboardEnabled;
};
