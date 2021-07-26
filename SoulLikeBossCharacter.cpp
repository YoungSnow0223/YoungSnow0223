// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulLikeBossCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ASoulLikeBossCharacter::ASoulLikeBossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Health = 100.0f;

	GetMovementComponent()->NavAgentProps.AgentHeight = 320.0f;
	GetMovementComponent()->NavAgentProps.AgentRadius = 160.0f;
}

float ASoulLikeBossCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= Damage;

	return Damage;
}

const float ASoulLikeBossCharacter::GetHealth()
{
	return Health;
}