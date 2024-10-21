// Fill out your copyright notice in the Description page of Project Settings.


#include "enemyDamage.h"
#include "characterDamage.h"

// Sets default values
AenemyDamage::AenemyDamage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("Damage Comp"));

}

// Called when the game starts or when spawned
void AenemyDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AenemyDamage::takeDamage(const UDamageInfo* damageInfo) {
	if (damageInfo) {
		if (damageComponent) {
			damageComponent->applyDamage(damageInfo);
		}
	}
}

void AenemyDamage::doDamage(AActor* target) {
	if (target) {
		UDamageInfo* damageInfo = NewObject<UDamageInfo>();

		damageInfo->damageAmount = 10.0;
		damageInfo->damageType = EDamageType::LightAttack;
		damageInfo->damageResponse = EDamageResponse::Melee;
		damageInfo->isIndestructible = false;

		AcharacterDamage* mainPlayer = Cast<AcharacterDamage>(target);
		if (mainPlayer) {
			mainPlayer->takeDamage(damageInfo);
		}
	}
}

/*// Called every frame
void AEnemyClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

