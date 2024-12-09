
// Wave Manager class for controling the wave based enemy spawning.
// Developer(s): Tristan Hughes 
// Last Updated: 11-26-24
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "enemycharacter1.h"
#include "waveManager.generated.h"

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
	TSubclassOf<Aenemycharacter1> blueprint;

	UPROPERTY(EditAnywhere, Category = "Wave Settings")
	float timeToSpawn;


};
