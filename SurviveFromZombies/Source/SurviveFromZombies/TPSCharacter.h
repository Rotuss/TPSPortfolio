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
	// 입력
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
	// 크로스헤어
	// ====================================================================================================
	void CalculateCrossHairSpread(float DeltaTime);
	
	// 크로스헤어 fire 설정
	void StartCrossHairFire();

	UFUNCTION()
	void FinishCrossHairFire();
	// ====================================================================================================


	// ====================================================================================================
	// 자동연사
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
	// 자동연사
	// ====================================================================================================
	// SetTimer을 위한 FTimerHandle, 현재 FireKey가 눌려있는 중인지 확인하는 bool, 현재 Fire가 가능한 상황인지 확인하는 bool, SetTimer에서 함수 호출하기까지의 시간
	FTimerHandle AutoFireTimer;
	bool bFiringKey;
	bool bShouldFire;
	float AutoFireRate;
	// ====================================================================================================


	// ====================================================================================================
	// 카메라 Field Of View
	// ====================================================================================================
	float CameraFOV;
	float CameraZoomFOV;
	float CameraCurrentFOV;

	// Aiming시 줌 스피드
	float ZoomInterpSpeed;
	// ====================================================================================================


	// ====================================================================================================
	// 크로스헤어
	// ====================================================================================================
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairSpread;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairVelocity;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairAim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Crosshair, meta = (AllowPrivateAccess = true))
	float CrossHairFire;

	// CrossHairFire 설정을 위한 추가 요소(SetTimer을 위한 FTimerHandle, 현재 Fire 중인지 확인하는 bool, SetTimer에서 함수 호출하기까지의 시간)
	FTimerHandle CrossHairFireTimer;
	bool bFiring;
	float FireTimeDuration;
	// ====================================================================================================

	// ====================================================================================================
	// 아이템 위젯 상호작용
	// ====================================================================================================
	UPROPERTY(VisibleAnywhere, Category = Item)
	class ATPSItem* TraceItemLastFrame;
	
	// 아이템 상호작용 가능 여부를 파악하기 위한 bool
	bool bShouldTraceForItem;
	// 오버랩 된 아이템 수 파악을 위한 int
	int OverlappedItemCount;
	// ====================================================================================================


	// ====================================================================================================
	// 입력
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
