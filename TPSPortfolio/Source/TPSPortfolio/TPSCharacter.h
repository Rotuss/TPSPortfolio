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
	// ��������Ʈ ���� Ȯ�ο�
	/** ������Ƽ ���ø����̼� */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** �ִ� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	/** ���� ü�� ����*/
	UFUNCTION(BlueprintPure, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** ���� ü�� ����. ���� 0�� MaxHealth ���̷� ���������ϰ� OnHealthUpdate�� ȣ���մϴ�. ���������� ȣ��Ǿ�� �մϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** ������� �޴� �̺�Ʈ. APawn���� �������̵�˴ϴ�.*/
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
	// ��������Ʈ ���� Ȯ�ο�
	/** �÷��̾��� �ִ� ü��. ü���� �ִ��Դϴ�. �� ���� ���� �� ���۵Ǵ� ĳ������ ü�� ���Դϴ�.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth;

	/** �÷��̾��� ���� ü��. 0�� �Ǹ� ���� ������ ���ֵ˴ϴ�.*/
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

	/** ���� ü�¿� ������ ���濡 ���� RepNotify*/
	UFUNCTION()
		void OnRep_CurrentHealth();

	/** ������Ʈ�Ǵ� ü�¿� ����. ���������� ���� ��� ȣ��, Ŭ���̾�Ʈ������ RepNotify�� �����Ͽ� ȣ��*/
	void OnHealthUpdate();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay|Projectile")
		TSubclassOf<class ATPSAmmo> ProjectileClass;

	/** �߻� ������, ������ ��. �׽�Ʈ �߻�ü�� �߻� �ӵ��� �����ϴ� �� ��������, ���� �Լ��� �߰����� SpawnProjectile�� �Է¿� ���� ���ε����� �ʰ� �ϴ� ���ҵ� �մϴ�.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float FireRate;

	/** true�� ��� �߻�ü�� �߻��ϴ� ���μ��� �����Դϴ�. */
	bool bIsFiringWeapon;

	/** ���� �߻� ���� �Լ�*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StartFire(const FInputActionValue& Value);

	/** ���� �߻� ���� �Լ�. ȣ��Ǹ� �÷��̾ StartFire�� �ٽ� ����� �� �ֽ��ϴ�.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void StopFire();

	/** �߻�ü�� �����ϴ� ���� �Լ�*/
	UFUNCTION(Server, Reliable)
		void HandleFire();

	/** ���� ���̿� �߻� �ӵ� �����̸� �ִ� Ÿ�̸� �ڵ�*/
	FTimerHandle FiringTimer;
	//========================================================================


private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
		USkeletalMeshComponent* Weapon;

	// �Է� ����
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