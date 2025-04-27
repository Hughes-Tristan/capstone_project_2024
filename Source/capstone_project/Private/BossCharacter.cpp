// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "developmentCharacterTH.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("DamageComponent"));
    
    
    audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
    audioComponent->SetupAttachment(RootComponent);
    audioComponent->bAutoActivate = false;
    canAttack = true;
    cooldownTime = 1.0f;
    minPitch = 0.6f;
    maxPitch = 1.4f;

    attackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("attackSphere"));
    attackSphere->SetCollisionProfileName(TEXT("collisionOverlap"));

    attackSphere->SetSphereRadius(150.0f);
    attackSphere->SetGenerateOverlapEvents(true);
    attackSphere->SetupAttachment(RootComponent);
    attackSphere->OnComponentBeginOverlap.AddDynamic(this, &ABossCharacter::OnAttackRangeOverlapBegin);
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
    
    damageComponent->setMaxHealth(1000.0f);
    damageComponent->setHealth(1000.0f);

    TArray<UPrimitiveComponent*> componentsArray;
    GetComponents<UPrimitiveComponent>(componentsArray);
    for (int32 i = 0; i < componentsArray.Num(); ++i) {
        UPrimitiveComponent* component = componentsArray[i];
        if (component) {
            component->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
        }
    }
	
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
   /* if (HealthbarHUD)
    {
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

            if (Distance <= 1500.0f)
            {
                HealthbarHUD->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                HealthbarHUD->SetVisibility(ESlateVisibility::Hidden);
            }
        }
    }*/
}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

/*
void ABossCharacter::applyDamage(const UdamageInfo* damageInfo)
{
    if (damageComponent)
    {
        damageComponent->applyDamage(damageInfo);
    }
}

void ABossCharacter::applyHealth(float healthAmount)
{
    if (damageComponent)
    {
        damageComponent->applyHealth(healthAmount);
    }
}
*/

void ABossCharacter::takeDamage(const UdamageInfo* damageInfo, float damage) {
    if (damageInfo) {
        if (damageComponent) {
            damageComponent->applyDamage(damageInfo, damage);
        }
    }
}

void ABossCharacter::doDamage(AActor* target, float damageAmount) {
    if (target) {

        UdamageInfo* damageInfo = NewObject<UdamageInfo>();

        damageInfo->damageAmount = damageAmount;
        //damageInfo->damageType = EDamageType::HeavyAttack;
        damageInfo->damageResponse = EDamageResponse::Melee;
        damageInfo->isIndestructible = false;
        damageInfo->attackingActor = this;

        AdevelopmentCharacter* mainPlayer = Cast<AdevelopmentCharacter>(target);
        if (mainPlayer) {
            mainPlayer->takeDamage(damageInfo, damageAmount);
        }
    }
}

void ABossCharacter::onSound(USoundCue* sound) {
    if (sound && audioComponent)
    {
        float randomPitch = FMath::RandRange(minPitch, maxPitch);

        audioComponent->SetSound(sound);
        audioComponent->SetPitchMultiplier(randomPitch);
        audioComponent->Play();
    }
}

void ABossCharacter::onAttackHit(float damage) {
    UE_LOG(LogTemp, Warning, TEXT("OnAttackHit called on %s!"), *GetName());
    if (currentTarget && canAttack)
    {
        doDamage(currentTarget, damage);
        if (attackHitSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                attackHitSound,
                currentTarget->GetActorLocation(),
                1.0f,
                FMath::RandRange(minPitch, maxPitch)
            );
        }
        if (attackCameraShake)
        {
            AdevelopmentCharacter* playerChararacter = Cast<AdevelopmentCharacter>(currentTarget);
            if (playerChararacter)
            {
                APlayerController* playerController = Cast<APlayerController>(playerChararacter->GetController());
                if (playerController)
                {
                    playerController->ClientStartCameraShake(attackCameraShake, 10.0f);
                }
            }
        }
    }
}

void ABossCharacter::OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
    AdevelopmentCharacter* mainCharacter;
    if (OtherActor && OtherActor != this) {

        mainCharacter = Cast<AdevelopmentCharacter>(OtherActor);
        if (mainCharacter)
        {
            currentTarget = mainCharacter;
        }
    }
}

void ABossCharacter::shouldAttack() {
    canAttack = true;
}



/*float ABossCharacter::getHealth() const
{
    return damageComponent ? damageComponent->getHealth() : 0.f;
}

float ABossCharacter::getMaxHealth() const
{
    return damageComponent ? damageComponent->getMaxHealth() : 1000.f;
}
*/
