// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeAgroCheck.h"
#include "SoulLike.h"
#include "SoulLikeAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BehaviorTree.h"
#include "SoulLikeBossCharacter.h"
#include "DrawDebugHelpers.h"


void USoulLikeAgroCheck::TickNode(UBehaviorTreeComponent&OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
	{
		ThisTree = OwnerComp.GetCurrentTree();
		ThisController = Cast<ASoulLikeAIController>(OwnerComp.GetAIOwner());
		ThisAICharacter = Cast<ASoulLikeBossCharacter>(ThisController->GetPawn());

		if (ThisTree == nullptr || ThisController == nullptr || ThisAICharacter == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Warning Agro Service performed on invalid AI"));
			return;
		}
	}

	// trace complex�� ������ �����ϸ鼭 sweep params ����ü�� �ʱ�ȭ�ϱ�
	FCollisionQueryParams SphereSweepParams(FName(TEXT("AgroCheckSweep")), true, ThisAICharacter);

	FCollisionObjectQueryParams ObjectQuery(ECC_GameTraceChannel1);

	// Hit Result �ʱ�ȭ�ϱ�
	FHitResult HitOut(ForceInit);

	// �ùٸ� ������ �����ϴ��� Ȯ���ϱ� ���� ����� ��ü�� �׸���
	DrawDebugSphere(ThisAICharacter->GetWorld(), ThisAICharacter->GetActorLocation(), 2000, 12, FColor::Red, false, 4.0f);

	bool bResult = ThisAICharacter->GetWorld()->SweepSingleByObjectType(HitOut, ThisAICharacter->GetActorLocation(),
		ThisAICharacter->GetActorLocation() + FVector(0.0f, 0.0f, 10.0f), FQuat(), ObjectQuery, FCollisionShape::MakeSphere(1500), SphereSweepParams);

	if (bResult)
	{
		ThisController->GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), HitOut.GetActor());
		ThisController->GetBlackboard()->SetValueAsVector(TEXT("HomeLocation"), ThisAICharacter->GetActorLocation());
		ThisController->GetBlackboard()->SetValueAsVector(TEXT("TargetLocation"), HitOut.GetActor()->GetActorLocation());
	}
	else
	{
		ThisController->GetBlackboard()->SetValueAsObject(TEXT("TargetToFollow"), nullptr);
	}

}