// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSCharacter.generated.h"

UCLASS()
class SURVIVEFROMZOMBIES_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	void Move(const FInputActionValue& Value);
	void Sight(const FInputActionValue& Value);
	void Fire(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY()
	class ATPSWeapon* CurWeapon;

	// ют╥б
	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputMappingContext* TPSContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* SightAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* FireAction;

};
