// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TPSAnimInstance.h"
#include "TPSWeapon.h"

// ����
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("Character"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Characters/TPS/TPSCharacter/Swat.Swat'"));
	if (true == SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	//static ConstructorHelpers::FClassFinder<UAnimInstance> BP_CHARACTER_ANIM(TEXT("AnimBlueprint'/Game/Characters/TPS/Animation/ABP_TPS.ABP_TPS_C'"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_CHARACTER_ANIM(TEXT("AnimBlueprint'/Game/Characters/TPS/Animation/ABP_TPSCharacter.ABP_TPSCharacter_C'"));
	if (BP_CHARACTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_CHARACTER_ANIM.Class);
	} 
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 100.0f), FRotator(0.0f, -50.0f, 0.0f));
	// �� ���� ȸ�� ���
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	// �����Ʈ �������� ȸ�� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	// ���� ���� ����
	GetCharacterMovement()->JumpZVelocity = 800.0f;
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	// �Է� ����
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>DEFAULT_CONTEXT
	(TEXT("/Game/Input/IMC_Character.IMC_Character"));
	if (true == DEFAULT_CONTEXT.Succeeded())
	{
		DefaultContext = DEFAULT_CONTEXT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CharacterSight
	(TEXT("/Game/Input/IA_CharacterSight.IA_CharacterSight"));
	if (true == IA_CharacterSight.Succeeded())
	{
		SightAction = IA_CharacterSight.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CharacterMove
	(TEXT("/Game/Input/IA_CharacterMove.IA_CharacterMove"));
	if (true == IA_CharacterMove.Succeeded())
	{
		MoveAction = IA_CharacterMove.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CharacterWalkRun
	(TEXT("/Game/Input/IA_CharacterRun.IA_CharacterRun"));
	if (true == IA_CharacterWalkRun.Succeeded())
	{
		WalkRunAction = IA_CharacterWalkRun.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CharacterJump
	(TEXT("/Game/Input/IA_CharacterJump.IA_CharacterJump"));
	if (true == IA_CharacterJump.Succeeded())
	{
		JumpAction = IA_CharacterJump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_CharacterShoot
	(TEXT("/Game/Input/IA_CharacterShoot.IA_CharacterShoot"));
	if (true == IA_CharacterShoot.Succeeded())
	{
		ShootAction = IA_CharacterShoot.Object;
	}

	// ����
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	// �߻� �ӵ� �ʱ�ȭ
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

// ����
void ATPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//���� ü�� ���ø�����Ʈ
	DOREPLIFETIME(ATPSCharacter, CurrentHealth);
}

// ����
void ATPSCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

// ����
float ATPSCharacter::TakeDamage(float DamageTaken, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(DefaultContext, 0);
		}
	}

	FName WeaponSocket(TEXT("hand_rSocket"));
	CurWeapon = GetWorld()->SpawnActor<ATPSWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurWeapon->SetOwner(this);
	}
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*auto TPSAnimInstance = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != TPSAnimInstance)
	{
		TPSAnimInstance->SetSpeed(GetVelocity().Size());
	}*/


	//UE_LOG(LogTemp, Warning, TEXT("MainMaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Owner : %s"), *GetOwner()->GetName());
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(SightAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Sight);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		EnhancedInputComponent->BindAction(WalkRunAction, ETriggerEvent::Started, this, &ATPSCharacter::Run);
		EnhancedInputComponent->BindAction(WalkRunAction, ETriggerEvent::Completed, this, &ATPSCharacter::Walk);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATPSCharacter::Shoot);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ATPSCharacter::StartFire);
	}
}

void ATPSCharacter::Sight(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		AddControllerYawInput(MovementVector.X);
		AddControllerPitchInput(MovementVector.Y * -1.0f);

		//UE_LOG(LogTemp, Warning, TEXT("Input SightAction"));
	}
}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		AddMovementInput(FRotationMatrix(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f))).GetUnitAxis(EAxis::X), MovementVector.Y);
		AddMovementInput(FRotationMatrix(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f))).GetUnitAxis(EAxis::Y), MovementVector.X);

		//UE_LOG(LogTemp, Warning, TEXT("Input MoveAction"));
	}
}

void ATPSCharacter::Walk(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Walk"));
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	UE_LOG(LogTemp, Warning, TEXT("WalkMaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ATPSCharacter::Run(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Run"));
	if (true == Value.Get<bool>())
	{
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		UE_LOG(LogTemp, Warning, TEXT("RunMaxWalkSpeed : %f"), GetCharacterMovement()->MaxWalkSpeed);
	}
}

void ATPSCharacter::Shoot(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		if (nullptr != CurWeapon)
		{
			CurWeapon->PullTrigger();
		}
	}
}

// ����
void ATPSCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

// ����
void ATPSCharacter::OnHealthUpdate()
{
	// Ŭ���̾�Ʈ ���� �Լ� ���
	if (true == IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	// ���� ���� �Լ� ���
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	// ��� �ӽſ��� ����Ǵ� �Լ� 
	/*
		���⿡ ����� �Ǵ� ����� ����� �߻��ϴ� Ư�� �Լ� ��� ��ġ
	*/
}

// ����
void ATPSCharacter::StartFire(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>() && false == bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		GetWorld()->GetTimerManager().SetTimer(FiringTimer, this, &ATPSCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

// ����
void ATPSCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

// ����
void ATPSCharacter::HandleFire_Implementation()
{
	if (nullptr != CurWeapon)
	{
		CurWeapon->PullTrigger();
	}
}

