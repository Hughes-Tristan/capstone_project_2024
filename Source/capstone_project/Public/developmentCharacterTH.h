
// Main Character Class for controlling character movement and actions
// Base Class is Unreal Engines Third Person Template Class
// Developer(s): Tristan Hughes (designed from third person template class as a base for this code)
// Last Updated: 11-26-24

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Logging/LogMacros.h"
#include "damageInfo.h"
#include "damageComponent.h"
#include "enemycharacter1.h"
#include "Blueprint/UserWidget.h"
#include "Templates/SubclassOf.h"


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
	
	// 
	virtual void Tick(float time) override;
	
	// function controls player rotation
	void orientPlayerRotation();

	// functions for controlling player actions
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

	//UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> healthWidget;

	UUserWidget* widgetInstance;

	float meleeCooldown;

private:
	// private variables associated with character actions
	UPROPERTY(BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Crouch", meta = (AllowPrivateAccess = "true"))
	bool isCrouching;

	UPROPERTY(BlueprintReadWrite, Category = "Melee", meta = (AllowPrivateAccess = "true"))
	float meleeDamageRange = 70.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	UdamageComponent* damageComponent;

	FTimerHandle timerHandle;
	float meleeTimer;
	bool canMelee;
	void shouldMelee();

	float setCurrentLength = 250.0f;
	float setTargetLength = 250.0f;
	
};



