// Fill out your copyright notice in the Description page of Project Settings.

#include "developmentCharacterTH.h"


// Template Base Code: Copyright Epic Games, Inc. All Rights Reserved.
// Template base code used from third person character template used and expanded upon with compliance to Epic Games Unreal Engine EULA

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

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
	CameraBoom->SocketOffset = FVector(0.0f,55.0f,70.0f);
	FRotator cameraRotation(0.0f, -10.0f, 0.0f);

	currentState = EPlayerState::Unarmed;
	//FollowCamera->AddRelativeRotation(cameraRotation);
	
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
		// crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::shouldCrouch);

		// change animation state for development
		EnhancedInputComponent->BindAction(SwitchAnimState, ETriggerEvent::Started, this, &AdevelopmentCharacter::setAnimationState);

		// sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AdevelopmentCharacter::startSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AdevelopmentCharacter::stopSprinting);

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

void AdevelopmentCharacter::Tick(float time) {
	Super::Tick(time);
	orientPlayerRotation();
	setCurrentLength = setSmoothArmLength(setCurrentLength, setTargetLength, time);
	CameraBoom->TargetArmLength = setCurrentLength;
}

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

void AdevelopmentCharacter::setAnimationState(const FInputActionValue& Value) {
	if (currentState == EPlayerState::Unarmed) {
		currentState = EPlayerState::Rifle;
	}
	else {
		currentState = EPlayerState::Unarmed;
	}
}

void AdevelopmentCharacter::startSprinting(const FInputActionValue& Value) {
	if (isCrouching == false) {
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		moveSpeed = 600.f;
	}
}

void AdevelopmentCharacter::stopSprinting(const FInputActionValue& Value) {
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	moveSpeed = 300.f;
}

float AdevelopmentCharacter::setSmoothArmLength(float currentLength, float targetLength, float timeDelta) {
	float newLength;
	newLength = FMath::FInterpTo(currentLength, targetLength, timeDelta, 10.0f);

	return newLength;
}

