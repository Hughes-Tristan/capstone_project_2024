
/*******************************************************************************************
*
*   Wave Manager v1.0.0 - Wave Manager class for controling the wave based enemy spawning.
*
*   Last Modified: 4-5-25
*
*   MODULE USAGE:
*	** Module usage section WIP **
*
*   DISCLAIMER: The "Module Usage" section of this header comment was generated with the assistance of generative AI.
*
*   LICENSE: Personal Use
*
*   Copyright © 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmarterEnemy.h"
#include "waveManager.generated.h"

// this class is designed for managing the wave system
UCLASS()
class CAPSTONE_PROJECT_API AwaveManager : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AwaveManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// functions for controlling the state of a wave
	void startWave();
	void endWave();
	void enemyDeath();
	void spawnBP();


	// variables for tracking certain aspects of the wavemanager
	int enemyCount, total, deathTotal, waveNumber;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	int minEnemies;
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	int maxEnemies;

	FTimerHandle timerHandle;
	
	UPROPERTY()
	TArray<AActor*> enemies;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	TArray<AActor*> spawnPoints;
	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	TSubclassOf<ASmarterEnemy> blueprint;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float timeToSpawn;


};
