
/*******************************************************************************************
*
*   Main Character v1.0.0 - Main Character Class for Controlling Character Movement and Actions
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
*   Copyright � 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*   This software is based on Unreal Engine (� 1998-2025 Epic Games, Inc.), and is subject to the terms of Unreal Engine's End User License Agreement (EULA).
*   Unauthorized use, reproduction, or distribution of this code, or parts of it, without proper authorization is prohibited.
*   
*   Unreal Engine�s base code and components are used under the Unreal Engine License, which can be reviewed at:
*   https://www.unrealengine.com/en-US/eula
* 
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Logging/LogMacros.h"
#include "damageInfo.h"
#include "damageComponent.h"
#include "enemycharacter1.h"
#include "Templates/SubclassOf.h"

#include "StaminaComponent.h"
#include "StaminaBar.h"
#include "Components/ProgressBar.h"


#include "Sound/SoundCue.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "developmentCharacterTH.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UNiagaraSystem;
class UNiagaraComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateDevelopmentCharacter, Log, All);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// this is an enum for controlling the player state
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed State"),
	Melee UMETA(DisplayName = "Melee State"),
	Rifle UMETA(DisplayName = "Rifle State"),
	Pistol UMETA(DisplayName = "Pistol State"),
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class AdevelopmentCharacter : public ACharacter
{

	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
	//
	// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// input actions for given movements
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* crouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* switchAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* sprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* meleeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* dashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* blockAction;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Tristan Hughes
	// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	AdevelopmentCharacter();


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// To add mapping context
	virtual void BeginPlay() override;

	//virtual void tick() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Trisan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// called every frame
	virtual void Tick(float time) override;
	
	// function controls player rotation
	void orientPlayerRotation();

	// functions for controlling player actions
	virtual void Jump() override;

	void shouldCrouch(const FInputActionValue& Value);
	void startSprinting(const FInputActionValue& Value);
	void stopSprinting(const FInputActionValue& Value);

	void startBlocking(const FInputActionValue& Value);
	void stopBlocking(const FInputActionValue& Value);

	void meleeAttack();
	void setAnimationState(const FInputActionValue& Value);
	float setSmoothArmLength(float currentLength, float targetLength, float timeDelta);

	// fucntions for using the modular damage system
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void takeDamage(const UdamageInfo* damageInfo, float damage);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void doDamage(AActor* target, float damageAmount);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage")
	float damageValue;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void shouldAnimate(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Character Status")
	bool isDead();

	UFUNCTION(BlueprintCallable, Category = "Input")
	void shouldDisableInput();

	// function for controlling the playerstate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
	EPlayerState currentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* attackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	FTimerHandle attackDelayHandle;

	//functions for controlling stamina
	UFUNCTION(BlueprintCallable, Category = "Stamina")
	float getStaminaAmount() const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool isStaminaAvailable(float staminaNeeded) const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool tryUseStamina(float staminaNeeded);


	float meleeCooldown;

	UFUNCTION(BlueprintCallable, Category = "MeleeFlag")
	void setCanMelee(bool shouldAllowMelee);

	UFUNCTION(BlueprintCallable, Category = "DamageFloat")
	void setDamageValue(float damageFloat);

	UFUNCTION(BlueprintCallable, Category = "BlockFlag")
	void setCanBlock(bool shouldAllowBlock);

	UFUNCTION(BlueprintCallable, Category = "SprintFlag")
	void setCanSprint(bool shouldAllowSprint);

	UFUNCTION(BlueprintCallable, Category = "SprintFlag")
	void setchildFriendlyFlag(bool shouldBeChildFriendly);

	// punching flag and animations
	//UPROPERTY(BlueprintReadWrite)
	//bool canPunch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* leftPunchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* rightPunchMontage;

	// dash function
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void performDash(const FInputActionValue& Value);

	// block flag
	UFUNCTION(BlueprintCallable, Category = "Block")
	bool getIsBlocking() const;

private:
	// private variables associated with character actions
	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool isCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Sprint", meta = (AllowPrivateAccess = "true"))
	bool isSprinting;

	UPROPERTY(BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float meleeDamageRange = 55.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UdamageComponent* damageComponent;

	FTimerHandle timerHandle;
	float meleeTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool canMelee;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool canBlock;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool canSprint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool childFriendlyFlag;

	void shouldMelee();

	bool isAttacking;
	FTimerHandle meleeCooldownTimeHandle;

	void animationEnded();

	float setCurrentLength = 250.0f;
	float setTargetLength = 250.0f;

	// stamina variables
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaminaComponent* staminaComponent;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float staminaCostEverySecond;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float meleeStaminaCost;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float jumpStaminaCost;

	void sprintStamina();
	FTimerHandle sprintStaminaTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStaminaBar> staminaBarClass;

	UPROPERTY()
	class UStaminaBar* staminaBarWidget;

	float displayedStamina;

	FVector2D defaultBarScale = FVector2D(1.0f, 1.0f);
    
    //Take Damage HUD
    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> DamageVignetteClass;
    
    UUserWidget* HitReactWidget;

	// audio variables
	UPROPERTY(EditAnywhere)
	float staminaSmoothingSpeed = 4.0f;

	UPROPERTY(EditAnywhere)
	USoundCue* meleeSound;

	UPROPERTY(EditAnywhere)
	USoundCue* meleeHitSound;

	UPROPERTY(EditAnywhere)
	UAudioComponent* audioComp;

	UPROPERTY(EditAnywhere)
	float meleeAudioDelay;

	FTimerHandle meleeAudioTimerHandle;

	void playAudio(USoundCue* activeSound, float minPitch, float maxPitch);

	// particle variables
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* meleeSwing;

	UPROPERTY(EditAnywhere)
	UParticleSystem* meleeHitParticle;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* particleComp;

	// socket variable
	UPROPERTY(EditAnywhere)
	FName weaponSocket;

	// effects methods

	void playSwingEffect(USoundCue* swingSound, UNiagaraSystem* swingEffect, FName socketName, float minSoundPitch, float maxSoundPitch);

	void playHitEffect(const FVector& hitLocation, USoundCue* hitSound, UParticleSystem* hitParticle, float cameraShakeIntensity, float minPitch, float maxPitch);

	// camerashake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> meleeCameraShake;


	// punching
	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* punchSound;

	UPROPERTY(EditAnywhere, Category = "Sounds", meta = (AllowPrivateAccess = "true"))
	USoundCue* punchHitSound;

	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* punchSwingEffect;

	UPROPERTY(EditAnywhere, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* punchHitParticle;

	UPROPERTY(EditAnywhere, Category = "Punch", meta = (AllowPrivateAccess = "true"))
	float punchDamageRange;

	UPROPERTY(EditAnywhere, Category = "Punch", meta = (AllowPrivateAccess = "true"))
	float punchDamageAmount;

	UAnimMontage* getRandomPunchAnimation();


	// dash variables
	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashDistance;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashDuration;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float dashStaminaCost;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue* dashSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UNiagaraSystem* dashEffect;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TSubclassOf<UCameraShakeBase> dashCameraShake;

	bool canDash;

	UPROPERTY(EditAnywhere, Category = "Dash", meta = (AllowPrivateAccess = "true"))
	float dashCooldown;

	bool isDashing;

	FTimerHandle dashCooldownTH;
	FTimerHandle dashDurationTH;
	FVector dashDirection;

	// dash functions
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void endDash();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void resetDashCooldown();

	void playDashEffects();

	// blocking properties
	UPROPERTY(BlueprintReadWrite, Category = "Block", meta = (AllowPrivateAccess = "true"))
	bool blocking;

	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
	float blockDamageReduction;

	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (AllowPrivateAccess = "true"))
	float blockStaminaDrainRate;

	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* blockMontage;

	FTimerHandle blockStaminaTH;
	void drainBlockStamina();

	//helper
	void initiateBlocking();

	UPROPERTY(EditAnywhere, Category = "Block", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* shieldHitMontage;

	UPROPERTY(EditAnywhere, Category = "Audio", meta = (AllowPrivateAccess = "true"))
	USoundCue* shieldHitSound;

	void playShieldHitEffects(const FVector& hitLocation);

	UPROPERTY(BlueprintReadWrite, Category = "Block", meta = (AllowPrivateAccess = "true"))
	bool inHitReaction;

	FTimerHandle hitReactionTimerHandle;

};



