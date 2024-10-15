// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "damageComponent.h"
#include "mainCharacter.h"
#include "EnemyClass.generated.h"

UCLASS()
class CAPSTONE_PROJECT_API AEnemyClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyClass();
	void takeDamage(const UDamageInfo& damageInfo);
	void doDamage(AActor* target);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UdamageComponent* damageComponent;

/*public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	*/
};
