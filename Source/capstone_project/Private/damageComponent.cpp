
/*******************************************************************************************
*
*   Damage Component v1.0.0 - Damage Component Class for Modular Damage System
*
*   Last Modified: 12-10-24
*
*   MODULE USAGE:
*	** Module usage section WIP **
*
*   DISCLAIMER: The "Module Usage" section of this header comment was generated with the assistance of generative AI.
*
*   LICENSE: Personal Use
*
*   Copyright (c) 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#include "damageComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "enemycharacter1.h"
#include "GameFramework/CharacterMovementComponent.h"

// this is the constructor for setting default values to an object of this clas
// sets the ticked every frame component, default health value, and death boolean value.
UdamageComponent::UdamageComponent() {
	
	// if you want to use the tick component function set this to true, false should help performance
	PrimaryComponentTick.bCanEverTick = false;
	health = 100.0;
	maxHealth = 100.0;
	isDead = false;
}


// this function is called when the game starts
void UdamageComponent::BeginPlay() {
	Super::BeginPlay();
}

// this function is designed to heal an actor
// if the player is dead then return
// the ammount of health you want to be healed by is applied to the health
// the amount of health you can get is clamped betweeen 0 and 100
void UdamageComponent::applyHealth(float healthAmount) {

	if (isDead) {
		return;
	}

	health += healthAmount;
	health = FMath::Clamp(health, 0, maxHealth);
}


// this function is designed to do direct damage to the actor
// based on the damageamount do damage to health and check for if the player is dead or not
void UdamageComponent::applyDirectDamage(float damageAmount) {
	health -= damageAmount;
	if (health <= 0) {
		health = 0;
		death();
		isDead = true;
	}
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

// this is a setter function for setting a new health value
// clamps the new health between 0 and the max health
// also sets the dead flag
void UdamageComponent::setHealth(float newHealth) {
	health = FMath::Clamp(newHealth, 0.0f, maxHealth);
	if (health <= 0) {
		health = 0;
		isDead = true;
	} else {
		isDead = false;
	}
}

// this is a setter function for setting a new max health
// it sets only if the value is above 0 and if the health is greater than the max health then set the max health to the health
void UdamageComponent::setMaxHealth(float newMaxHealth) {
	if (newMaxHealth > 0.0f) {
		maxHealth = newMaxHealth;
		if (health > maxHealth) {
			health = maxHealth;
		}
	}

}

// this is called every frame, should be set to true in the constructor
//void UdamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
//}


