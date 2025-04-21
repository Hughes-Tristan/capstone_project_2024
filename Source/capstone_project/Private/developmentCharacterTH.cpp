
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
*   Copyright © 2025 Tristan Hughes and 2025 UNR Capstone Team 10. All Rights Reserved.
*   This software is based on Unreal Engine (© 1998-2025 Epic Games, Inc.), and is subject to the terms of Unreal Engine's End User License Agreement (EULA).
*   Unauthorized use, reproduction, or distribution of this code, or parts of it, without proper authorization is prohibited.
*
*   Unreal Engine’s base code and components are used under the Unreal Engine License, which can be reviewed at:
*   https://www.unrealengine.com/en-US/eula
*
*   Unauthorized copying of this file, via any medium is strictly prohibited
*   This project is personal and confidential unless stated otherwise.
*   Permission for use in any form must be granted in writing by Tristan Hughes and the 2025 UNR Capstone Team 10.
*
**********************************************************************************************/

#include "developmentCharacterTH.h"


// all the necessary libraries
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "damageInfo.h"
#include "enemycharacter1.h"
#include "SmarterEnemy.h"
#include "enemyPatrolCharacter.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/OverlapResult.h"
#include "Engine/HitResult.h"

#include "TimerManager.h"

DEFINE_LOG_CATEGORY(LogTemplateDevelopmentCharacter);

//////////////////////////////////////////////////////////////////////////
// Acapstone_projectCharacter

AdevelopmentCharacter::AdevelopmentCharacter()
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set size for collision capsule
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// initial camera state settings
	CameraBoom->SocketOffset = FVector(0.0f,55.0f,70.0f);
	FRotator cameraRotation(0.0f, -10.0f, 0.0f);

	// inistial player state
	currentState = EPlayerState::Unarmed;
	//FollowCamera->AddRelativeRotation(cameraRotation);

	damageComponent = CreateDefaultSubobject<UdamageComponent>(TEXT("damage component initialization"));

	meleeTimer = 0.05;
	canMelee = false;
	
	// stamina system default values
	staminaCostEverySecond = 5.0f;
	//meleeStaminaCost = 20.0f;
	jumpStaminaCost = 8.0f;

	staminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina Component"));
	displayedStamina = 1.0f;
	//widgetInstance = nullptr;
	isAttacking = false;

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComp->SetupAttachment(RootComponent);
	audioComp->bAutoActivate = false;

	meleeAudioDelay = 0.2f;

	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	particleComp->SetupAttachment(RootComponent);
	particleComp->bAutoActivate = false;

	weaponSocket = FName("weapon_r");

	// camera boom improvements
	CameraBoom->bDoCollisionTest = true;
	CameraBoom->ProbeChannel = ECC_Camera;
	CameraBoom->ProbeSize = 12.0f;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;
	CameraBoom->CameraRotationLagSpeed = 10.0f;

	// punching and damage defaults
	damageValue = 50.0f;

	punchDamageAmount = 15.0f;
	punchDamageRange = 60.0f;

	// dash variable defaults
	canDash = true;
	isDashing = false;

	dashStaminaCost = 15.0f;
	dashDuration = 1.0f;
	dashDistance = 1000.0f;
	dashCooldown = 1.0f;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AdevelopmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// this block of code checks if a stamina bar class is valid
	// it trys to get a player controller associated with the actor then if the controller is valid
	// create the stamina bar widget and if the widget is valid add the widget to the viewport
	// if the stamina component is valid set the displayed stamina variable to the current stamina percent
	if (staminaBarClass)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController)
		{
			staminaBarWidget = CreateWidget<UStaminaBar>(playerController, staminaBarClass);
			if (staminaBarWidget)
			{
				staminaBarWidget->AddToViewport();
			}
		}
	}

	if (staminaComponent) {
		displayedStamina = staminaComponent->getStaminaAmount();
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Input
void AdevelopmentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AdevelopmentCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AdevelopmentCharacter::Look);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// input mappings for specific actions
		// crouching
		EnhancedInputComponent->BindAction(crouchAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::shouldCrouch);

		// change animation state for development
		EnhancedInputComponent->BindAction(switchAnimState, ETriggerEvent::Started, this, &AdevelopmentCharacter::setAnimationState);

		// sprinting
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::startSprinting);
		EnhancedInputComponent->BindAction(sprintAction, ETriggerEvent::Completed, this, &AdevelopmentCharacter::stopSprinting);

		// melee attack
		EnhancedInputComponent->BindAction(meleeAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::shouldAnimate);

		// dash mechanic
		EnhancedInputComponent->BindAction(dashAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::performDash);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Tristan Hughes
// 
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 	
	}
	else
	{
		UE_LOG(LogTemplateDevelopmentCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdevelopmentCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AdevelopmentCharacter::Look(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK ABOVE IS WRITTEN BY Unreal Engine
// 
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Trisan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Trisan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// this function is called every frame and it updates the character
// it also is being used to smoothly set our character arm length when the player transitions arm lengths
// it is also checking if the player is dead, if the player is dead then it should disable the input
// stamina system was added and general functionality for the stamina system have been included in this function
// if the stamina bar widget, stamina component, and stamina bar is valid then continue
// get current stamina percent, smoothly interpolate between the display and current stamina for smooth visual representation
// update stamina bar then change stamina bar color based on the current stamina
// then create a pulse affect that changes the opacity and scale when the stamina is less than 25 percent
void AdevelopmentCharacter::Tick(float time) {
	Super::Tick(time);
	orientPlayerRotation();
	setCurrentLength = setSmoothArmLength(setCurrentLength, setTargetLength, time);
	CameraBoom->TargetArmLength = setCurrentLength;

	if (isDead()) {
		shouldDisableInput();
	}
	
	if (staminaBarWidget && staminaComponent && staminaBarWidget->staminaBar) {

		float currentSTamina = staminaComponent->getStaminaAmount();

		displayedStamina = FMath::FInterpTo(
			displayedStamina,
			currentSTamina,
			time,
			staminaSmoothingSpeed
		);
		staminaBarWidget->staminaBar->SetPercent(displayedStamina);

		FLinearColor barColor = FLinearColor::LerpUsingHSV(
			FLinearColor(1.0f, 0.0f, 0.0f),
			FLinearColor(0.0f, 1.0f, 0.0f),
			displayedStamina
		);

		staminaBarWidget->staminaBar->SetFillColorAndOpacity(barColor);

		if (displayedStamina < 0.25f) {
			float pulseTime = GetWorld()->GetTimeSeconds() * 5.0f;
			float pulseValue = (FMath::Sin(pulseTime) * 0.2f) + 0.8f;
			staminaBarWidget->staminaBar->SetRenderOpacity(pulseValue);

			float pulseScale = (FMath::Sin(pulseTime) * 0.02f) + 1.0f;
			FVector2D updatedScale = defaultBarScale * pulseScale;

			FWidgetTransform transform;
			transform.Scale = FVector2d(pulseScale, pulseScale);
			staminaBarWidget->staminaBar->SetRenderTransform(transform);
		}
		else {
			staminaBarWidget->staminaBar->SetRenderOpacity(1.0f);
			FWidgetTransform transform;
			transform.Scale = defaultBarScale;
			staminaBarWidget->staminaBar->SetRenderTransform(transform);
		}
	}

	if (staminaComponent) {
		UE_LOG(LogTemp, Display, TEXT("Stamina: %f / %f (%.1f%%)"),
			staminaComponent->getStamina(),
			staminaComponent->getMaxStamina(),
			staminaComponent->getStaminaAmount() * 100.0f);
	}
}

// this function updates the player rotation based on his velocity
// if the character is not moving he has a freely rotating camera
// if the character is moving it should lock the player camera to the direction of the movement
void AdevelopmentCharacter::orientPlayerRotation() {

	FVector charVelocity = GetCharacterMovement()->Velocity;
	const float checkLimit = 0.1f;
	bool movementCheck = false;

	if (charVelocity.SizeSquared() > FMath::Square(checkLimit)) {
		movementCheck = true;
	} else {
		movementCheck = false;
	}

	if (movementCheck) {
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else {
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

// this function is used to handle the crouch action
// is crouching gets initialized to false but is triggered when the action is initiated
// crouch if not crouch and uncrouch when crouched
// changed walk speed and camera arm length based on state
void AdevelopmentCharacter::shouldCrouch(const FInputActionValue& Value) {
	if (isCrouching) {
		GetCharacterMovement()->UnCrouch();
		isCrouching = false;

		setTargetLength = 250.0f;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	} else {
		GetCharacterMovement()->Crouch();
		isCrouching = true; 

		setTargetLength = 200.0f;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}

// this function is used to control the player animation state
// it changes state when triggered
void AdevelopmentCharacter::setAnimationState(const FInputActionValue& Value) {
	if (currentState == EPlayerState::Unarmed) {
		currentState = EPlayerState::Melee;
	}
	else {
		currentState = EPlayerState::Unarmed;
	}
}

// this function is used to initiate the action of sprinting
// if the player is crouching then they cant sprint
void AdevelopmentCharacter::startSprinting(const FInputActionValue& Value) {
	if (isCrouching == false && tryUseStamina(staminaCostEverySecond * 0.5f)) {
		isSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
		moveSpeed = 750.f;

		GetWorldTimerManager().SetTimer(sprintStaminaTH, this, &AdevelopmentCharacter::sprintStamina, 0.1f, true);

		if (staminaComponent) {
			staminaComponent->setStaminaGain(false);
		}
	}
}

// this function is used to stop the aciton of sprinting and return to walking
void AdevelopmentCharacter::stopSprinting(const FInputActionValue& Value) {
	if (isSprinting) {
		isSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 350.f;
		moveSpeed = 350.f;

		GetWorldTimerManager().ClearTimer(sprintStaminaTH);

		if (staminaComponent) {
			staminaComponent->setStaminaGain(true);
		}
	}
}

//this function is used to control how smoothly the camera transitions from one length to another
float AdevelopmentCharacter::setSmoothArmLength(float currentLength, float targetLength, float timeDelta) {
	float newLength;
	newLength = FMath::FInterpTo(currentLength, targetLength, timeDelta, 10.0f);

	return newLength;
}

// this function is designed to handle the functionality for the player taking damage
// if the function receives damage info then apply damage using the modular damage system
void AdevelopmentCharacter::takeDamage(const UdamageInfo* damageInfo) {
	if (damageInfo) {
		if (damageComponent) {
			damageComponent->applyDamage(damageInfo);
		}
	}
}

// this function is designed to do damage to an actor
// if an actor is detected than setup damage info for the attack
// if the cast to the enemy character is successful it will do damage to the enemy
void AdevelopmentCharacter::doDamage(AActor* target, float damageAmount) {
	if (target) {
		
		UdamageInfo* damageInfo = NewObject<UdamageInfo>();

		damageInfo->damageAmount = damageAmount;
		//damageInfo->damageType = EDamageType::HeavyAttack;
		damageInfo->damageResponse = EDamageResponse::Melee;
		damageInfo->isIndestructible = false;
		damageInfo->attackingActor = this;

		Aenemycharacter1* enemyPresent = Cast<Aenemycharacter1>(target);

		if (enemyPresent) {
			enemyPresent->takeDamage(damageInfo);
		}

		ASmarterEnemy* smarterEnemy = Cast<ASmarterEnemy>(target);
		if (smarterEnemy) {
			smarterEnemy->takeDamage(damageInfo);
		}
	}
}


// this function is designed to perform a melee atack
// it perofrms a check for whether actors are ofverlapping within a radius
// if there is an enemy in the radius then damage gets applied to all the actors in the radius
// and a hit effect occurs at the hit location
// after a hit is complete it initials a cooldown timer
void AdevelopmentCharacter::meleeAttack() {
	TArray<FOverlapResult> storedHits;
	TSet<AActor*> actorOverlap;
	FCollisionQueryParams queryParams;
	FVector charPos;// forwardVector, centerOfSphere;
	float hitRadius;

	hitRadius = meleeDamageRange;
	charPos = GetActorLocation();
	queryParams.AddIgnoredActor(this);


	GetWorld()->OverlapMultiByChannel(storedHits, charPos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(hitRadius), queryParams);

	bool hitEnemy = false;
	FVector hitLocation = charPos;

	for (int32 i = storedHits.Num() - 1; i >= 0; --i) {
		const FOverlapResult& overlapResult = storedHits[i];
		AActor* enemy = overlapResult.GetActor();
		if (enemy && enemy != this) {

			if (!actorOverlap.Contains(enemy)) {

				actorOverlap.Add(enemy);
				if (canMelee) {
					damageValue = 65;
					doDamage(enemy, damageValue);

					hitEnemy = true;
					hitLocation = enemy->GetActorLocation();
					playHitEffect(hitLocation, meleeHitSound, meleeHitParticle, 1.0f, 0.6f, 1.4f);
				}
				else {
					damageValue = 20;
					doDamage(enemy, damageValue);

					hitEnemy = true;
					hitLocation = enemy->GetActorLocation();
					playHitEffect(hitLocation, punchHitSound, punchHitParticle, 0.75f, .8f, 1.1f);
				}

				FColor sphereColor = storedHits.Num() > 0 ? FColor::Red : FColor::Green;
				DrawDebugSphere(GetWorld(), charPos, hitRadius, 12, FColor::Green, false, 1.0, 0, 1.0);
			}
		}
	}
}




// this function is used to trigger an animation for the attacking control
// it also sets the swinging particle effect and sound to play
// it also sets a timer for reseting the attack cooldown with respect to the animation
// cooldown for attacking has also been added to fix a melee bug
void AdevelopmentCharacter::shouldAnimate(const FInputActionValue& Value) {
	if (!isAttacking) {
		float montageTime, attackDelay;
		if (attackMontage && canMelee) {
			isAttacking = true;
			
			montageTime = PlayAnimMontage(attackMontage, 2.0f);

			attackDelay = montageTime / 5;

			playSwingEffect(meleeSound, meleeSwing, weaponSocket, 0.6f, 1.4f);

			GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AdevelopmentCharacter::meleeAttack, attackDelay, false);

			GetWorldTimerManager().SetTimer(meleeCooldownTimeHandle, this, &AdevelopmentCharacter::animationEnded, montageTime * 0.25f, false);
		} else if (!canMelee) {
			isAttacking = true;

			UAnimMontage* punchAnimation = getRandomPunchAnimation();
			if (punchAnimation) {
				montageTime = PlayAnimMontage(punchAnimation, 1.0f);
				attackDelay = montageTime / 5;
				FName handSocket = FName("hand_r");
				if (punchAnimation == leftPunchMontage) {
					handSocket = FName("hand_l");
				}
				playSwingEffect(punchSound, punchSwingEffect, handSocket, 0.8f, 1.1f);
				GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AdevelopmentCharacter::meleeAttack, attackDelay, false);

				GetWorldTimerManager().SetTimer(meleeCooldownTimeHandle, this, &AdevelopmentCharacter::animationEnded, montageTime * 0.25f, false);
			}
		}
	}
}

// this function is a setter for reseting the melee cooldown
void AdevelopmentCharacter::shouldMelee() {
	canMelee = true;
}

// this function is used to check if the character is dead as long as a damage component exists
bool AdevelopmentCharacter::isDead() {
	if (damageComponent) {
		return damageComponent->isDead;
	}
	return false;
}

// this function is used to disable input when the character is dead
void AdevelopmentCharacter::shouldDisableInput() {
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController) {
		playerController->DisableInput(playerController);
	}
}

// this is a getter function that gets the current stamina as a percent
float AdevelopmentCharacter::getStaminaAmount() const {
	if (staminaComponent) {
		return staminaComponent->getStaminaAmount();
	}
	return 0.0f;
}

//this function  is used to check if the character has enough stamina to perform an action given the required stamina needed
bool AdevelopmentCharacter::isStaminaAvailable(float staminaNeeded) const {
	if (staminaComponent) {
		return staminaComponent->isStaminaAvailable(staminaNeeded);
	}
	return false;
}

// this function is used to attempt to use a given stamina amount
// if there is enough stamina then take the stamina amount, otherwise return false
bool AdevelopmentCharacter::tryUseStamina(float staminaNeeded) {
	if (staminaComponent && staminaComponent->isStaminaAvailable(staminaNeeded)) {
		staminaComponent->takeStamina(staminaNeeded);
		return true;
	}
	return false;
}

// this functions is used to handle stamina usage
// if there is a stamina component and the character is sprinting then take the the stamina from the available stamina
// otherwise force sprinting to stop if there is no stamina
void AdevelopmentCharacter::sprintStamina() {
	if (staminaComponent && isSprinting) {
		float staminaCost = staminaCostEverySecond * 0.1f;
		if (staminaComponent->isStaminaAvailable(staminaCost)) {
			staminaComponent->takeStamina(staminaCost);
		}
		else {
			stopSprinting(FInputActionValue());
		}
	}
}

// this is a override funtion for the jump function and only calls the base jump functions if there is enough stamina to jump
void AdevelopmentCharacter::Jump() {
	if (tryUseStamina(jumpStaminaCost)) {
		Super::Jump();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("not enough stamina to jump"));
	}
}

// this is a function sets the isAttacking flag to false when the animation is ended
void AdevelopmentCharacter::animationEnded() {
	isAttacking = false;
}


// this function is used to control the melee swing effects like sound and particle animation
// it sets a timer to give the swing audio a slight delay and play the sound, it then sets up a niagara particle effect
// it spawns the particle affect attached to the weapon socket, it then uses a timer to decide the duration the effect should play before being destroyed
void  AdevelopmentCharacter::playSwingEffect(USoundCue* swingSound, UNiagaraSystem* swingEffect, FName socketName, float minSoundPitch, float maxSoundPitch) {
	GetWorldTimerManager().SetTimer(
		meleeAudioTimerHandle,
		[this, swingSound, minSoundPitch, maxSoundPitch]() { this->playAudio(swingSound, minSoundPitch, maxSoundPitch); },
		meleeAudioDelay,
		false
	);
	
	if (swingEffect) {

		UNiagaraComponent* niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			swingEffect,
			GetMesh(),
			socketName,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (niagaraComp) {
			float duration = 0.5f;
			FTimerHandle niagaraTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(niagaraTimerHandle, FTimerDelegate::CreateLambda([niagaraComp]() {
				if (niagaraComp)
				{
					niagaraComp->DestroyComponent();
				}
				}), duration, false);
		}
	}
}



// this function is used to control the melee hit effects such as sound and particle effects when a hit is detected
// if there is a hit sound present then play the melee sound at the hit location with a random pitch range
// if there is a hit partcile present the spawn the particle at the hit location at double the scale of the original particle
// if there is a playerControler and melee camera shake is present then start a camera shake
void  AdevelopmentCharacter::playHitEffect(const FVector& hitLocation, USoundCue* hitSound, UParticleSystem* hitParticle, float cameraShakeIntensity, float minPitch, float maxPitch) {
	if (hitSound) {
		float randomPitch = FMath::RandRange(minPitch, maxPitch);
		UGameplayStatics::PlaySoundAtLocation(
			this,
			hitSound,
			hitLocation,
			1.0f,
			randomPitch
		);
	}

	FVector particleScale(2.0f, 2.0f, 2.0f);
	if (hitParticle) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			hitParticle,
			hitLocation,
			FRotator::ZeroRotator,
			particleScale,
			true
		);
	}

	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController && meleeCameraShake) {
		playerController->ClientStartCameraShake(meleeCameraShake, cameraShakeIntensity);
	}
}

// this function is used to play the melee audio at a random pitch range 
void AdevelopmentCharacter::playAudio(USoundCue* activeSound, float minPitch, float maxPitch) {
	if (activeSound && audioComp) {
		float randomPitch = FMath::RandRange(minPitch, maxPitch);

		audioComp->SetSound(activeSound);
		audioComp->SetPitchMultiplier(randomPitch);
		audioComp->Play();
	}
}

// this is a setter function for allowing melee
void AdevelopmentCharacter::setCanMelee(bool shouldAllowMelee) {
	canMelee = shouldAllowMelee;
}

// this function is used to randomize whether the character does a left or right punch when not holding a melee
UAnimMontage* AdevelopmentCharacter::getRandomPunchAnimation() {
	if (leftPunchMontage && rightPunchMontage) {
		if (FMath::RandBool()) {
			return leftPunchMontage;
		} else {
			return rightPunchMontage;
		}
	}
	else {
		return nullptr;
	}
}

// this function is used to perform the dash
// it checks to make sure you can dash otherwise it returns
// then it gets the direction of movement, sets the dashing to true and the can dash to false
// then it sets the dash force and it launches the player by a multiple of the dash force with the direction
// then it sets a timer for ending the dash and reseting the dash cooldown
void AdevelopmentCharacter::performDash(const FInputActionValue& Value) {
	if (!canDash || !tryUseStamina(dashStaminaCost) || isCrouching || isDashing) {
		return;
	}

	FVector forwardVector = GetActorForwardVector();
	FVector velocityVector = GetVelocity();

	if (velocityVector.SizeSquared() > 1.0f) {
		velocityVector.Z = 0;
		dashDirection = velocityVector.GetSafeNormal();
	}
	else {
		dashDirection = forwardVector;
	}

	isDashing = true;
	canDash = false;

	float dashForce = 1600.0f;
	GetCharacterMovement()->Launch(dashDirection * dashForce);


	playDashEffects();
	GetWorldTimerManager().SetTimer(dashDurationTH, this, &AdevelopmentCharacter::endDash, dashDuration, false);
	GetWorldTimerManager().SetTimer(dashCooldownTH, this, &AdevelopmentCharacter::resetDashCooldown, dashCooldown, false);
}

void AdevelopmentCharacter::endDash() {
	isDashing = false;
	//GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

// this function is a flag reset for the canDash variable
void AdevelopmentCharacter::resetDashCooldown() {
	canDash = true;
}


// this function is used to control the sound, particles, and camera shake for the dash effect
void AdevelopmentCharacter::playDashEffects() {
	playAudio(dashSound, 0.9f, 1.1f);
	if (dashEffect) {

		UNiagaraComponent* niagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			dashEffect,
			GetMesh(),
			FName("torso"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (niagaraComp) {
			float duration = 0.5f;
			FTimerHandle niagaraTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(niagaraTimerHandle, FTimerDelegate::CreateLambda([niagaraComp]() {
				if (niagaraComp)
				{
					niagaraComp->DestroyComponent();
				}
				}), duration, false);
		}
	}
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController && dashCameraShake) {
		playerController->ClientStartCameraShake(dashCameraShake, 0.5f);
	}
}
