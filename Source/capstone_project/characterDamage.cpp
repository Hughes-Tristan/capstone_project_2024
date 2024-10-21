// Fill out your copyright notice in the Description page of Project Settings.


#include "characterDamage.h"
#include "enemyDamage.h"

// Sets default values
AcharacterDamage::AcharacterDamage()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("Damage Comp"));
}

// Called when the game starts or when spawned
void AcharacterDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

void AcharacterDamage::takeDamage(const UDamageInfo* damageInfo) {
	if (damageInfo) {
		if (damageComponent) {
			damageComponent->applyDamage(damageInfo);
		}
	}
}

void AcharacterDamage::doDamage(AActor* target) {
	if(target) {
		UDamageInfo* damageInfo = NewObject<UDamageInfo>();

		damageInfo->damageAmount = 10.0;
		damageInfo->damageType = EDamageType::LightAttack;
		damageInfo->damageResponse = EDamageResponse::Melee;
		damageInfo->isIndestructible = false;

		AenemyDamage* enemyPresent = Cast<AenemyDamage>(target);
		
		if(enemyPresent) {
			enemyPresent->takeDamage(damageInfo);
		}
	}
}

// Called every frame
/*void AmainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}*/

// Called to bind functionality to input/
/*void AmainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}*/
