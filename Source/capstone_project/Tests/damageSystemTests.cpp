// Tristan Hughes

#include "CoreMinimal.h"
#include "misc/AutomationTest.h"
#include "damageComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDamageComponentTest, "DamageComponentTest", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FDamageComponentTest::RunTest(const FString& Parameters) {
    // Arrange
    UdamageComponent* DamageComponent = NewObject<UdamageComponent>();

    // Act/Assert
    TestNotNull("DamageComponent should be valid", &DamageComponent);
    TestEqual("Default health should be 100", DamageComponent->getHealth(), 100.0f);
    TestFalse("Default isDead should be false", DamageComponent->isDead);

    DamageComponent->applyDirectDamage(50.0f);
    TestEqual("Health should be 50 after taking 50 damage", DamageComponent->getHealth(), 50.0f);

    return true;
}
