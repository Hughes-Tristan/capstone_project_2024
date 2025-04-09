// Joey Bertrand

#include "CoreMinimal.h"
#include "misc/AutomationTest.h"
#include "waveManager.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWaveManagerTest, "WaveManagerTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FWaveManagerTest::RunTest(const FString& Parameters) {
    // Arrange
    AwaveManager* WaveManager = NewObject<AwaveManager>();

    // Act/Assert
    TestNotNull("WaveManager should be valid", &WaveManager);
    TestEqual("Default wave number should be 1", WaveManager->waveNumber, 1);
    TestEqual("Default min enemies should be 5", WaveManager->minEnemies, 5);
    TestEqual("Default max enemies should be 10", WaveManager->maxEnemies, 10);
    TestEqual("Default time to spawn should be 0.5", WaveManager->timeToSpawn, 0.5f);

    return true;
}
