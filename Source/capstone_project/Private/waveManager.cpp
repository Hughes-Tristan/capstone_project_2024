
/*******************************************************************************************
*
*   Wave Manager v1.0.0 - Wave Manager class for controling the wave based enemy spawning.
*
*   Last Modified: 11-26-24
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

#include "waveManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"

// constructor to store initial values 
AwaveManager::AwaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	enemyCount = 0;
	//deathTotal 
	waveNumber = 1;
	minEnemies = 2;
	maxEnemies = 5;
	timeToSpawn = 0.5f;

	//maxEnemiesAtOnce = 8;
	maxEnemiesTotal = 30;
	difficultyCurve = 1.4f;
	//countScaling = 1.2f;
	waveCooldownTime = 5.0f;
	enemyCleanupDelay = 3.0f;


	bossRoundInterval = 5;
	isBossRound = false;
}

// Called when the game starts or when spawned
void AwaveManager::BeginPlay()
{
	Super::BeginPlay();
	startWave();
}

// Called every frame
void AwaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//spawnBP();

}

// this function is intended for starting new waves
// assigns a number of enemies to spawn based of a given range
// starts a timer to spawn new enemies based on an interval
void AwaveManager::startWave() {
	isBossRound = (waveNumber % bossRoundInterval == 0);
	if (isBossRound) {
        total = FMath::RandRange(minEnemies, maxEnemies) + 1;
        enemyCount = 0;
        deathTotal = 0;
        UE_LOG(LogTemp, Warning, TEXT("wave: %d   total enemies spawning: %d"), waveNumber, total);
        timeToSpawn = FMath::Max(0.5f - (waveNumber * 0.02f), 0.1f);
        GetWorldTimerManager().SetTimer(timerHandle, this, &AwaveManager::spawnBP, timeToSpawn, true);
		spawnBoss();
	}
	else {
		total = FMath::RandRange(minEnemies, maxEnemies);
		enemyCount = 0;
		deathTotal = 0;
		UE_LOG(LogTemp, Warning, TEXT("wave: %d   total enemies spawning: %d"), waveNumber, total);
		timeToSpawn = FMath::Max(0.5f - (waveNumber * 0.02f), 0.1f);
		GetWorldTimerManager().SetTimer(timerHandle, this, &AwaveManager::spawnBP, timeToSpawn, true);
	}
}

// this function is designed to spawn the enemy blueprint
// it quickly checks if the correct number of enemies have spawned
// if so clear the timer so no more spawn then return
// then choose a random spawnpoint, get location and rotation, then set you spawn parameters
// then spawn an enemy
// if it spawns successfully then add to the list of enemies spawned, increase enemy tally and link the wavemanager
// then find the valid ai controller class from the blueprint defaults
// if we find a valid controller then spawn it
void AwaveManager::spawnBP() {
	
	if (total <= enemyCount) {
		GetWorldTimerManager().ClearTimer(timerHandle);
		return;
	}

	int i = FMath::RandRange(0, spawnPoints.Num()- 1);
	AActor* spawnPoint = spawnPoints[i];

	FRotator spawnRotation = spawnPoint->GetActorRotation();
	FVector spawnLocation = spawnPoint->GetActorLocation();
	FActorSpawnParameters spawnParameters;
    ASmarterEnemy* enemySpawned = GetWorld()->SpawnActor<ASmarterEnemy>(blueprint, spawnLocation, spawnRotation, spawnParameters);

    if (enemySpawned) {
        enemies.Add(enemySpawned);
        enemyCount++;
        enemySpawned->waveManager = this;

        UCharacterMovementComponent* movementComp = enemySpawned->GetCharacterMovement();
        if (movementComp) {
            movementComp->GravityScale = 1.0f;
            //movementComp->SetMovementMode(MOVE_Walking);
        }

		// the next 10 lines gets the ai controller class from the blueprint defined in the blueprints defaults
		// these lines were written with the assistance of generative AI
        UClass* blueprintClass = enemySpawned->GetClass();
        UClass* aiControllerClass = nullptr;

        for (TFieldIterator<FObjectProperty> PropIt(blueprintClass); PropIt; ++PropIt) {
            FObjectProperty* Property = *PropIt;
            if (Property->GetName() == "AIControllerClass") {
                aiControllerClass = Cast<UClass>(Property->GetObjectPropertyValue_InContainer(blueprintClass->GetDefaultObject()));
                break;
            }
        }

        if (aiControllerClass) {
            AAIController* aiController = GetWorld()->SpawnActor<AAIController>(aiControllerClass);
            if (aiController) {
                if (enemySpawned->GetController()) {
                    enemySpawned->GetController()->UnPossess();
					UE_LOG(LogTemp, Warning, TEXT("unpossesed"));
                }
                aiController->Possess(enemySpawned);
				UE_LOG(LogTemp, Warning, TEXT("successfully possesed"));
            }
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Could not find AI controller class for enemy %s"), *enemySpawned->GetName());
        }
    }
}


// this function is designed to be called on an enemy death
// it tallies the deaths and if there are the same amount of dead as there are total in the wave
// then end the wave
void AwaveManager::enemyDeath() {
	deathTotal++;

	if (total <= deathTotal) {
		GetWorldTimerManager().SetTimer(cleanupTimerHandle, this, &AwaveManager::endWave, enemyCleanupDelay, false);
	}
}

// this function is intended to be used for ending waves
// once this is called it will destroy all the enemies, empty the list for new wave
// it will thenincrease the wave number and range numbers before starting a new wave
void AwaveManager::endWave() {
	int destroyedCount = 0;
	for (int i = 0; i < enemies.Num(); ++i) {
		AActor* enemy = enemies[i];
		if (enemy) {
			enemy->Destroy();
			destroyedCount++;
		}
	}
	enemies.Empty();
	GetWorldTimerManager().SetTimer(betweenWavesTimer, this, &AwaveManager::prepareNextWave, waveCooldownTime, false);
}

void AwaveManager::prepareNextWave() {
	//float difficultyMultiplier = FMath::Pow(waveNumber, 1.0f / difficultyCurve);

	int oldMinEnemies = minEnemies;
	int oldMaxEnemies = maxEnemies;

	minEnemies = FMath::Min(minEnemies + 1, maxEnemiesTotal);
	maxEnemies = FMath::Min(maxEnemies + 2, maxEnemiesTotal);

	if (minEnemies > maxEnemies) {
		minEnemies = maxEnemies;
	}

	waveNumber++;
	//UpdateWaveCounterUI();
	startWave();
}

// this function is used to spawn the boss character
// it chooses a random spawn point, spawns the boss, and sets up the ai controller
void AwaveManager::spawnBoss() {
    int i = FMath::RandRange(0, spawnPoints.Num() - 1);
    AActor* spawnPoint = spawnPoints[i];

    FRotator spawnRotation = spawnPoint->GetActorRotation();
    FVector spawnLocation = spawnPoint->GetActorLocation();
    FActorSpawnParameters spawnParameters;
    spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    ABossCharacter* bossSpawned = GetWorld()->SpawnActor<ABossCharacter>(bossBlueprint, spawnLocation, spawnRotation, spawnParameters);

    if (bossSpawned) {
        enemies.Add(bossSpawned);
        enemyCount++;


        UClass* blueprintClass = bossSpawned->GetClass();
        UClass* aiControllerClass = nullptr;

        for (TFieldIterator<FObjectProperty> PropIt(blueprintClass); PropIt; ++PropIt) {
            FObjectProperty* Property = *PropIt;
            if (Property->GetName() == "AIControllerClass") {
                aiControllerClass = Cast<UClass>(Property->GetObjectPropertyValue_InContainer(blueprintClass->GetDefaultObject()));
                break;
            }
        }

        if (aiControllerClass) {
            AAIController* aiController = GetWorld()->SpawnActor<AAIController>(aiControllerClass);
            if (aiController) {
                if (bossSpawned->GetController()) {
                    bossSpawned->GetController()->UnPossess();
                }
                aiController->Possess(bossSpawned);
            }
        }
    }
}

