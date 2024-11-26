// 
// Code by: Tristan Hughes
// Date: 10-14-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/

#include "damageComponent.h"

// Sets default values for this component's properties
UdamageComponent::UdamageComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// Default Health Value
	health = 100.0;
}


// Called when the game starts
void UdamageComponent::BeginPlay() {
	Super::BeginPlay();
}

void UdamageComponent::applyHealth(float healthAmount) {
	health += healthAmount;
}

void UdamageComponent::applyDamage(const UdamageInfo* damageInfo) {
	if (damageInfo->isIndestructible || isDead) {
		return;
	}

	ACharacter* player = Cast<ACharacter>(GetOwner());
	if (!damageInfo->isIndestructible) {
		float damageApplied = damageInfo->damageAmount;
		switch (damageInfo->damageType) {
		case EDamageType::None:
			damageApplied = 0.0;
			TEXT("No Damage Taken");
			break;

		case EDamageType::LightAttack:
			damageApplied = 0.24;
			break;

		case EDamageType::HeavyAttack:
			damageApplied = 0.49;
			break;

		default:
			break;
		}

		health -= damageApplied;

		if (health <= 0) {
			health = 0;
			death();
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
}

float UdamageComponent::getHealth() const {
	return health;
}

float UdamageComponent::getMaxHealth() const {
	return maxHealth;
}

void UdamageComponent::death() {
	isDead = true;
	ACharacter* characterCast = Cast<ACharacter>(GetOwner());
	characterCast->GetMesh()->SetSimulatePhysics(true);
}


/*// Called every frame
void UdamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}*/

