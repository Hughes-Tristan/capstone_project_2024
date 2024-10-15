// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyClass.h"

// Sets default values
AEnemyClass::AEnemyClass()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("Damage Comp"));

}

// Called when the game starts or when spawned
void AEnemyClass::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyClass::takeDamage(const UDamageInfo& damageInfo) {
	if (damageComponent) {
		damageComponent->applyDamage(damageInfo);
	}
}

void doDamage(AActor* target) {
	if (target) {
		UDamageInfo damageInfo;
		damageInfo.damageAmount = 10.0;
		damageInfo.damageType = EDamageType::LightAttack;
		damageInfo.damageResponse = EDamageResponse::Melee;
		damageInfo.isIndestructible = false;

		AmainCharacter* mainPlayer = Cast<AmainCharacter>(target);
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

