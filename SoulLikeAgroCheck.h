// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "SoulLikeAgroCheck.generated.h"

/**
 * 
 */
UCLASS()
class SOULLIKE_API USoulLikeAgroCheck : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UBehaviorTree* ThisTree;
	class ASoulLikeAIController* ThisController;
	class ASoulLikeBossCharacter* ThisAICharacter;
};
