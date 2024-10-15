// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damageComponent.h"
#include "mainCharacter.generated.h"

UCLASS()
class CAPSTONE_PROJECT_API AmainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AmainCharacter();
	void takeDamage(const UDamageInfo& damageInfo);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UdamageComponent* damageComponent;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
