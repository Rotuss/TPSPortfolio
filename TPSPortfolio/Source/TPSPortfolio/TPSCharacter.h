// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPortfolio.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSCharacter.generated.h"

class ATPSWeapon;

UCLASS(config = Game)
class TPSPORTFOLIO_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

	//========================================================================
	// 데디케이트 서버 확인용
	/** 프로퍼티 리플리케이션 */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** 최대 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** 현재 체력 게터*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** 현재 체력 세터. 값을 0과 MaxHealth 사이로 범위제한하고 OnHealthUpdate를 호출합니다. 서버에서만 호출되어야 합니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** 대미지를 받는 이벤트. APawn에서 오버라이드됩니다.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//========================================================================

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Sight(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	//========================================================================
	// 데디케이트 서버 확인용
	/** 플레이어의 최대 체력. 체력의 최댓값입니다. 이 값은 스폰 시 시작되는 캐릭터의 체력 값입니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** 플레이어의 현재 체력. 0이 되면 죽은 것으로 간주됩니다.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

	/** 현재 체력에 가해진 변경에 대한 RepNotify*/
	UFUNCTION()
		void OnRep_CurrentHealth();

	/** 업데이트되는 체력에 반응. 서버에서는 수정 즉시 호출, 클라이언트에서는 RepNotify에 반응하여 호출*/
	void OnHealthUpdate();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<class ATPSAmmo> ProjectileClass;

	/** 발사 딜레이, 단위는 초. 테스트 발사체의 발사 속도를 제어하는 데 사용되지만, 서버 함수의 추가분이 SpawnProjectile을 입력에 직접 바인딩하지 않게 하는 역할도 합니다.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** true인 경우 발사체를 발사하는 프로세스 도중입니다. */
	bool bIsFiringWeapon;

	/** 무기 발사 시작 함수*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire(const FInputActionValue& Value);

	/** 무기 발사 종료 함수. 호출되면 플레이어가 StartFire를 다시 사용할 수 있습니다.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** 발사체를 스폰하는 서버 함수*/
	UFUNCTION(Server, Reliable)
		void HandleFire();

	/** 스폰 사이에 발사 속도 딜레이를 넣는 타이머 핸들*/
	FTimerHandle FiringTimer;
	//========================================================================


private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// 입력 매핑
	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputMappingContext* DefaultContext;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* SightAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* MoveAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* WalkRunAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* JumpAction;

	UPROPERTY(VisibleAnywhere, Category = Input)
		class UInputAction* ShootAction;

	UPROPERTY()
		ATPSWeapon* CurWeapon;

	UFUNCTION(Server, reliable)
	void ServerToClient_Walk();
	UFUNCTION(Server, reliable)
	void ServerToClient_Run();

	void Walk_Speed();
	void Run_Speed();
};