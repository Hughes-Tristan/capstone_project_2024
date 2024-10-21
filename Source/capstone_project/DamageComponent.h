// 
// Code by: Tristan Hughes
// Date: 10-14-24

/*
Look into copyright notice ? this comment was included when creating the class: Fill out your copyright
notice in the Description page of Project Settings.
*/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "DamageInfo.h" 
#include "damageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAPSTONE_PROJECT_API UdamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UdamageComponent();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void applyHealth(float healthAmount);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void applyDamage(const UDamageInfo* damageInfo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float health;
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
