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
	// �̵� ���� �迭
	UPROPERTY(EditAnywhere, Category = Logic) TArray<class ATargetPoint*> TargetArray;
	// �ȴ� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* walkingSound;
	// �ٴ� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* runningSound;
	// Ÿ�� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* hitEnemySound;
	// �´� ���带 ���� ����� ������Ʈ
	UPROPERTY(EditAnywhere, Category = Sound) UAudioComponent* damageSound;

protected:
	// �÷��̾�� ī�޶� ���� �Ÿ��� �����ϱ� ���� ī�޶� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class USpringArmComponent* CameraBoom;

	// Ÿ�� ��� ����� ī�޶�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera) class UCameraComponent* FollowCamera;

	// Character Attack1 montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
		class UAnimMontage* CharacterAttack1Montage;

public:

	// Ű���� �Է�����
	UFUNCTION(BlueprintCallable, Category = "Animation")
		void SetIsKeyboardEnabled(bool Enabled);

	// �÷��̾� �յ�, �¿��� Ÿ�� ��ġ �̵��� �����Ѵ�
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera") float BaseLookUpRate;
	

	/*UFUNCTION(BlueprintCallable, Category = "Animation")
		bool IsArmed();
		*/

	// �÷��̾ �����ڼ��� ���ٽ� ����
	/*UFUNCTION()
		void ArmPlayer();
		*/

	// Ư�� �ð� ���� idle ���ٽ��� ����
	UFUNCTION()
		void TriggerCountdownToIdle();

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trace")
		int32 MaxCountdownToIdle;
		*/



	// �÷��̾ ���� 1 ����
	void Attack1();
	// �÷��̾ ���� 1 �ߴ�
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
	// ĳ���� �� ��ġ ������
	short CurrentLocation;
	FVector DesiredLocation;
	bool bBeingPushed;

	//bool bIsArmed;
	//FTimerHandle ArmedToIdleTimerHandle;
	//int32 CountdownToIdle;

	bool IsKeyboardEnabled;
};
