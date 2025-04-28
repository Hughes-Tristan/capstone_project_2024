// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "damageComponent.h"
#include "damageInterface.h"
#include "damageInfo.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraShakeBase.h"
#include "Components/SphereComponent.h"
#include "Engine/World.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossCharacter.generated.h"

UCLASS(Blueprintable)
class CAPSTONE_PROJECT_API ABossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossCharacter();
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    
    //Damage System Boss Character interface
    /*
    virtual void applyHealth(float healthAmount);
    virtual void applyDamage(const UdamageInfo* damageInfo);
    //virtual float getHealth() const;
    //virtual float getMaxHealth() const;
    */

    //HitReact Montage Reference
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    UAnimMontage* HitreactMontage;
    
    //function for HitReact Montage
    UFUNCTION(BlueprintCallable, Category = "Combat")
    
    void PlayHitReactMontage();
    
    UPROPERTY(BlueprintReadWrite)
    bool BossIsReacting = false;
    
    UFUNCTION(BlueprintCallable, Category = "Damage")
    void takeDamage(const UdamageInfo* damageInfo, float damage);

    UFUNCTION(BlueprintCallable, Category = "Damage")
    void doDamage(AActor* target, float damageAmount);

    UPROPERTY(EditAnywhere, Category = "Audio")
    UAudioComponent* audioComponent;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* attackSound;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void onSound(USoundCue* sound);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void onAttackHit(float damage);

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* attackHitSound;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* jumpSound;

    UPROPERTY(EditAnywhere, Category = "Effects")
    TSubclassOf<UCameraShakeBase> attackCameraShake;

    bool canAttack;

    UPROPERTY(EditAnywhere, Category = "Audio")
    float minPitch = 0.8f;

    UPROPERTY(EditAnywhere, Category = "Audio")
    float maxPitch = 1.2f;

    FTimerHandle timerHandle;
    float cooldownTime;

    void shouldAttack();

    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    USphereComponent* attackSphere;

    UPROPERTY(BlueprintReadWrite, Category = "Animation")
    AActor* currentTarget;

    UFUNCTION()
    void OnAttackRangeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    //void HandleHealth();
    
private:
    UPROPERTY(BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
    UdamageComponent* damageComponent;
    

    
    //void UpdateBossHealthBarVisibility();

    
    //UPROPERTY(EditAnywhere, Category = "UI")
    //TSubclassOf<UUserWidget> HealthBarClass;
    
    //UUserWidget* HealthbarHUD;
};
