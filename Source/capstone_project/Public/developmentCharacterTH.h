// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "developmentCharacterTH.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateDevelopmentCharacter, Log, All);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CODE WITHIN THE BLOCK BELOW IS WRITTEN BY Tristan Hughes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Unarmed UMETA(DisplayName = "Unarmed State"),
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

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
	
	// Functions
	virtual void Tick(float time) override;
	
	void orientPlayerRotation();
	void shouldCrouch(const FInputActionValue& Value);
	void startSprinting(const FInputActionValue& Value);
	void stopSprinting(const FInputActionValue& Value);

	void setAnimationState(const FInputActionValue& Value);
	float setSmoothArmLength(float currentLength, float targetLength, float timeDelta);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation State")
	EPlayerState currentState;

private:
	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool isCrouching;

	float setCurrentLength = 250.0f;
	float setTargetLength = 250.0f;
	
};



