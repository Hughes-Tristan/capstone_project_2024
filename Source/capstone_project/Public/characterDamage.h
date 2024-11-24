// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "damageComponent.h"

class AenemyDamage;

#include "characterDamage.generated.h"

UCLASS()
class CAPSTONE_PROJECT_API AcharacterDamage : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AcharacterDamage();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void takeDamage(const UdamageInfo* damageInfo);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void doDamage(AActor* target);

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
