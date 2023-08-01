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

	void IncrementOverlappedItemCount(int Amount);
	void TraceForItem();

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

	FORCEINLINE bool GetAiming() const { return bAiming; }
	FORCEINLINE float GetCrossHairSpread() const { return CrossHairSpread; }

	FORCEINLINE int GetOverlappedItemCount() const { return OverlappedItemCount; }

protected:
	// ====================================================================================================
	// �Է�
	// ====================================================================================================
	void Move(const FInputActionValue& Value);
	void Sight(const FInputActionValue& Value);
	void Fire(/*const FInputActionValue& Value*/);
	void FireStart(const FInputActionValue& Value);
	void FireEnd(const FInputActionValue& Value);
	void AimingStart(const FInputActionValue& Value);
	void AimingEnd(const FInputActionValue& Value);
	// ====================================================================================================


	// ====================================================================================================
	// Aiming
	// ====================================================================================================
	void AimingInterpZoom(float DeltaTime);
	// ====================================================================================================


	// ====================================================================================================
	// ũ�ν����
	// ====================================================================================================
	void CalculateCrossHairSpread(float DeltaTime);
	
	// ũ�ν���� fire ����
	void StartCrossHairFire();

	UFUNCTION()
	void FinishCrossHairFire();
	// ====================================================================================================


	// ====================================================================================================
	// �ڵ�����
	// ====================================================================================================
	void FireStartTimer();
	
	UFUNCTION()
	void AutoFireReset();
	// ====================================================================================================

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY()
	class ATPSWeapon* CurWeapon;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = true))
	bool bAiming;


	// ====================================================================================================
	// �ڵ�����
	// ====================================================================================================
	// SetTimer�� ���� FTimerHandle, ���� FireKey�� �����ִ� ������ Ȯ���ϴ� bool, ���� Fire�� ������ ��Ȳ���� Ȯ���ϴ� bool, SetTimer���� �Լ� ȣ���ϱ������ �ð�
	FTimerHandle AutoFireTimer;
	bool bFiringKey;
	bool bShouldFire;
	float AutoFireRate;
	// ====================================================================================================


	// ====================================================================================================
	// ī�޶� Field Of View
	// ====================================================================================================
	float CameraFOV;
	float CameraZoomFOV;
	float CameraCurrentFOV;

	// Aiming�� �� ���ǵ�
	float ZoomInterpSpeed;
	// ====================================================================================================


	// ====================================================================================================
	// ũ�ν����
	// ====================================================================================================
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairSpread;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairVelocity;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairAim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairFire;

	// CrossHairFire ������ ���� �߰� ���(SetTimer�� ���� FTimerHandle, ���� Fire ������ Ȯ���ϴ� bool, SetTimer���� �Լ� ȣ���ϱ������ �ð�)
	FTimerHandle CrossHairFireTimer;
	bool bFiring;
	float FireTimeDuration;
	// ====================================================================================================

	// ====================================================================================================
	// ������ ���� ��ȣ�ۿ�
	// ====================================================================================================
	UPROPERTY(VisibleAnywhere, Category = Item)
	class ATPSItem* TraceItemLastFrame;
	
	// ������ ��ȣ�ۿ� ���� ���θ� �ľ��ϱ� ���� bool
	bool bShouldTraceForItem;
	// ������ �� ������ �� �ľ��� ���� int
	int OverlappedItemCount;
	// ====================================================================================================


	// ====================================================================================================
	// �Է�
	// ====================================================================================================
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

	UPROPERTY(VisibleAnywhere, Category = Input)
	class UInputAction* AimAction;
	// ====================================================================================================

};
