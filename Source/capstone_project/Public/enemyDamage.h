// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "damageComponent.h"

class AcharacterDamage;

#include "enemyDamage.generated.h"

UCLASS()
class CAPSTONE_PROJECT_API AenemyDamage : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AenemyDamage();

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void takeDamage(const UdamageInfo* damageInfo);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void doDamage(AActor* target);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

    
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UdamageComponent* damageComponent;

	/*public:
		// Called every frame
		virtual void Tick(float DeltaTime) override;
		*/
};
