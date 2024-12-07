
// Damage Component Class for Modular Damage System
// Developer(s): Tristan Hughes 
// Last Updated: 11-26-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/

#include "damageComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "enemycharacter1.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UdamageComponent::UdamageComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Default Health Value
	health = 100.0;
	isDead = false;
}


// Called when the game starts
void UdamageComponent::BeginPlay() {
	Super::BeginPlay();
}

// this function is designed to heal an actor
// the ammount of health you want to be healed by is applied to the health
void UdamageComponent::applyHealth(float healthAmount) {
	health += healthAmount;
}

// this function is used to apply damage to an actor
// it receives damage info and check if the actor is already dead
// it then casts to a character 
// afterwords it uses a switch statement to handle different attacks and damages associated with those attack
// once that is done we apply the damage and check to see if the player should be dead or not
void UdamageComponent::applyDamage(const UdamageInfo* damageInfo) {
	if (damageInfo->isIndestructible || isDead) {
		return;
	}

	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (!player) {
		return;
	}
	
	if (!damageInfo->isIndestructible) {
		float damageApplied = damageInfo->damageAmount;
		switch (damageInfo->damageType) {
		case EDamageType::None:
			damageApplied = 0.0;
			TEXT("No Damage Taken");
			break;
            


		case EDamageType::EnemyAttack:
			damageApplied = 5;
			break;

		case EDamageType::LightAttack:
			damageApplied = 24;
			break;

		case EDamageType::HeavyAttack:
			damageApplied = 51;
			break;

		default:
			break;
		}

		health -= damageApplied;

		//UE_LOG(LogTemp,TEXT("damage applied: %f"), damageApplied);
		//UE_LOG(LogTemp, TEXT("remaining health: %f"), health);

		if (health <= 0) {
			health = 0;
			death();
			isDead = true;
		}
		else {
			switch (damageInfo->damageResponse) {
			case EDamageResponse::Melee:
				TEXT("MELEE");
				break;
			default:
				break;
			}
		}

	}
	
	/*
	health -= damageInfo->damageAmount;

	if (health <= 0) {
		health = 0;
		death();
	}*/
}

// this is a getter that returns the health value
float UdamageComponent::getHealth() const {
	return health;
}

// this is a getter that returns the maxhealth value
float UdamageComponent::getMaxHealth() const {
	return maxHealth;
}

// this function is responsible for handling death mechanics
// it is used to properly set the actor to ragdoll and for the capsule collision to be removed after death
void UdamageComponent::death() {
	
	ACharacter* characterCast = Cast<ACharacter>(GetOwner());
	characterCast->GetMesh()->SetSimulatePhysics(true);
	characterCast->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UCapsuleComponent* capsuleComponent = characterCast->GetCapsuleComponent();
	if (capsuleComponent) {
		capsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	characterCast->GetCharacterMovement()->DisableMovement();

	//Aenemycharacter1* enemyCharacter = Cast<Aenemycharacter1>(characterCast);
	//enemyCharacter->destroy();
	//characterCast->GetMesh()->WakeAllRigidBodies();
}

/*// Called every frame
void UdamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

