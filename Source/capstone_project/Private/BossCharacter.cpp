// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("DamageComponent"));
    
    

}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::applyDamage(const UdamageInfo* damageInfo)
{
    if (damageComponent)
    {
        damageComponent->applyDamage(damageInfo);
    }
}

void ABossCharacter::applyHealth(float healthAmount)
{
    if (damageComponent)
    {
        damageComponent->applyHealth(healthAmount);
    }
}

float ABossCharacter::getHealth() const
{
    return damageComponent ? damageComponent->getHealth() : 0.f;
}

float ABossCharacter::getMaxHealth() const
{
    return damageComponent ? damageComponent->getMaxHealth() : 1000.f;
}

