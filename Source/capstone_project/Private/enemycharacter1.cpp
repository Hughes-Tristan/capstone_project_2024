// Fill out your copyright notice in the Description page of Project Settings.


#include "enemycharacter1.h"

// Sets default values
Aenemycharacter1::Aenemycharacter1()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aenemycharacter1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aenemycharacter1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Aenemycharacter1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
