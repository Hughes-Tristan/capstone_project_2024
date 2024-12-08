
// Main Character Class for controlling character movement and actions
// Base Class is Unreal Engines Third Person Template Class
// Developer(s): Tristan Hughes (designed from third person template class as a base for this code)
// Last Updated: 11-26-24

#include "developmentCharacterTH.h"


// Template Base Code: Copyright Epic Games, Inc. All Rights Reserved.
// Template base code used from third person character template used and expanded upon with compliance to Epic Games Unreal Engine EULA

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
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
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
	canMelee = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AdevelopmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}


// Input
void AdevelopmentCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Unreal Engine
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::shouldCrouch);

		// change animation state for development
		EnhancedInputComponent->BindAction(SwitchAnimState, ETriggerEvent::Started, this, &AdevelopmentCharacter::setAnimationState);

		// sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::startSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AdevelopmentCharacter::stopSprinting);

		// melee attack
		EnhancedInputComponent->BindAction(MeleeAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::shouldAnimate);

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
// it also is being used to smoothing set our character arm length when the player transitions arm lengths
void AdevelopmentCharacter::Tick(float time) {
	Super::Tick(time);
	orientPlayerRotation();
	setCurrentLength = setSmoothArmLength(setCurrentLength, setTargetLength, time);
	CameraBoom->TargetArmLength = setCurrentLength;

	if (isDead()) {
		shouldDisableInput();
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
	}
	else {
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
	if (isCrouching == false) {
		GetCharacterMovement()->MaxWalkSpeed = 750.f;
		moveSpeed = 750.f;
	}
}

// this function is used to stop the aciton of sprinting and return to walking
void AdevelopmentCharacter::stopSprinting(const FInputActionValue& Value) {
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	moveSpeed = 350.f;
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
void AdevelopmentCharacter::doDamage(AActor* target) {
	if (target) {
		
		UdamageInfo* damageInfo = NewObject<UdamageInfo>();

		damageInfo->damageAmount = 10.0;
		damageInfo->damageType = EDamageType::HeavyAttack;
		damageInfo->damageResponse = EDamageResponse::Melee;
		damageInfo->isIndestructible = false;
		damageInfo->attackingActor = this;

		Aenemycharacter1* enemyPresent = Cast<Aenemycharacter1>(target);

		if (enemyPresent) {
			enemyPresent->takeDamage(damageInfo);
	
		}
	}
}


//this function is designed to perform a melee atack
// it perofrms a check for whether actors are ofverlapping withing a radius
// if there is an enemy in the radius  then damage gets applied to all the actors in the radius
// after a hit is complete in initials a cooldown timer
void AdevelopmentCharacter::meleeAttack() {
	TArray<FOverlapResult> storedHits;
	TSet<AActor*> actorOverlap;
	FCollisionQueryParams queryParams;
	FVector charPos, forwardVector, centerOfSphere;
	float hitRadius;

	hitRadius = meleeDamageRange;
	charPos = GetActorLocation();
	queryParams.AddIgnoredActor(this);
	if (canMelee) {


		GetWorld()->OverlapMultiByChannel(storedHits, charPos, FQuat::Identity, ECC_Pawn, FCollisionShape::MakeSphere(hitRadius), queryParams);

		for (int32 i = storedHits.Num() - 1; i >= 0; --i) {
			const FOverlapResult& overlapResult = storedHits[i];
			AActor* enemy = overlapResult.GetActor();
			if (enemy && enemy != this) {
				//doDamage(enemy);
		
				if (!actorOverlap.Contains(enemy)) {
					
					actorOverlap.Add(enemy);
					doDamage(enemy);
				}
				canMelee = false;
				GetWorldTimerManager().SetTimer(timerHandle, this, &AdevelopmentCharacter::shouldMelee, meleeTimer, false);

			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("attack cooldown"));

	}
	FColor sphereColor = storedHits.Num() > 0 ? FColor::Red : FColor::Green;
	DrawDebugSphere(GetWorld(), charPos, hitRadius, 12, FColor::Green, false, 1.0, 0, 1.0);

	
}

void AdevelopmentCharacter::shouldAnimate(const FInputActionValue& Value) {
	if (attackMontage) {
		float montageTime;
		montageTime = PlayAnimMontage(attackMontage, 2.0f);

		float attackDelay;
		attackDelay = montageTime /5;

		GetWorldTimerManager().SetTimer(attackDelayHandle, this, &AdevelopmentCharacter::meleeAttack, attackDelay, false);
	}
}

// this function is a setter for reseting the melee cooldown
void AdevelopmentCharacter::shouldMelee() {
	canMelee = true;
}

bool AdevelopmentCharacter::isDead() {
	if (damageComponent) {
		return damageComponent->isDead;
	}
	return false;
}

void AdevelopmentCharacter::shouldDisableInput() {
	APlayerController* playerController = Cast<APlayerController>(GetController());
	if (playerController) {
		playerController->DisableInput(playerController);
	}
}

