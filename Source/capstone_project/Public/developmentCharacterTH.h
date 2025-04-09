
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

	
	// input actions for given movements
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MeleeAction;

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
	void meleeAttack();
	void setAnimationState(const FInputActionValue& Value);
	float setSmoothArmLength(float currentLength, float targetLength, float timeDelta);

	// fucntions for using the modular damage system
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void takeDamage(const UdamageInfo* damageInfo);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void doDamage(AActor* target);

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

private:
	// private variables associated with character actions
	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool isCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Sprint", meta = (AllowPrivateAccess = "true"))
	bool isSprinting;

	UPROPERTY(BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float meleeDamageRange = 70.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UdamageComponent* damageComponent;

	FTimerHandle timerHandle;
	float meleeTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool canMelee;

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

	UPROPERTY(EditAnywhere)
	float minPitch;

	UPROPERTY(EditAnywhere)
	float maxPitch;

	FTimerHandle meleeAudioTimerHandle;

	void playDelayedAudio();

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

	void playSwingEffect();

	void playHitEffect(const FVector& hitLocation);

	// camerashake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> meleeCameraShake;
};



