// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TPSAnimInstance.h"
#include "TPSWeapon.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
	: bAiming(false)
	, CameraFOV(0.0f)
	, CameraZoomFOV(30.0f)
	, CameraCurrentFOV(0.0f)
	, ZoomInterpSpeed(30.0f)
	, CrossHairSpread(0.0f)
	, CrossHairVelocity(0.0f)
	, CrossHairAim(0.0f)
	, CrossHairFire(0.0f)
	, CrossHairFireTimer()
	, bFiring(false)
	, FireTimeDuration(0.05f)
	, AutoFireTimer()
	, bFiringKey(false)
	, bShouldFire(true)
	, AutoFireRate(0.1f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Character/TPS/Ch15_nonPBR.Ch15_nonPBR'"));
	if (true == SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> BP_CHARACTER_ANIM(TEXT("AnimBlueprint'/Game/Character/Animation/ABP_TPS.ABP_TPS_C'"));
	if (BP_CHARACTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BP_CHARACTER_ANIM.Class);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeLocation(FVector(0.0f, 50.0f, 80.0f));
	SpringArm->SetRelativeRotation(FRotator(0.0f, -30.0f, 0.0f));
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;

	// 무브먼트 방향으로 회전 조정
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	// 입력
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>IMC_TPS
	(TEXT("/Game/Input/IMC_TPS.IMC_TPS"));
	if (true == IMC_TPS.Succeeded())
	{
		TPSContext = IMC_TPS.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Move
	(TEXT("/Game/Input/IA_Move.IA_Move"));
	if (true == IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Sight
	(TEXT("/Game/Input/IA_Sight.IA_Sight"));
	if (true == IA_Sight.Succeeded())
	{
		SightAction = IA_Sight.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Jump
	(TEXT("/Game/Input/IA_Jump.IA_Jump"));
	if (true == IA_Jump.Succeeded())
	{
		JumpAction = IA_Jump.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Fire
	(TEXT("/Game/Input/IA_Fire.IA_Fire"));
	if (true == IA_Fire.Succeeded())
	{
		FireAction = IA_Fire.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction>IA_Aim
	(TEXT("/Game/Input/IA_Aim.IA_Aim"));
	if (true == IA_Aim.Succeeded())
	{
		AimAction = IA_Aim.Object;
	}

}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubSystem->AddMappingContext(TPSContext, 0);
		}
	}

	if (nullptr != Camera)
	{
		// 초기값 세팅
		CameraFOV = GetCamera()->FieldOfView;
		CameraCurrentFOV = CameraFOV;
	}

	FName WeaponSocket(TEXT("Weapon_socket"));
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

	// Aiming InterpTo
	AimingInterpZoom(DeltaTime);

	// 크로스헤어 확산 연산
	CalculateCrossHairSpread(DeltaTime);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		EnhancedInputComponent->BindAction(SightAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Sight);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATPSCharacter::Fire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ATPSCharacter::FireStart);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ATPSCharacter::FireEnd);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ATPSCharacter::AimingStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ATPSCharacter::AimingEnd);
	}

}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, ControllerRotation.Yaw, 0.0f);
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// 앞뒤(W,S)
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), MovementVector.Y /** GetWorld()->DeltaTimeSeconds*/);
		// 좌우(D,A)
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), MovementVector.X /** GetWorld()->DeltaTimeSeconds*/);

		//UE_LOG(LogTemp, Warning, TEXT("Input MoveAction"));
	}
}

void ATPSCharacter::Sight(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// 좌우
		AddControllerYawInput(MovementVector.X /** GetWorld()->DeltaTimeSeconds*/);
		// 상하
		AddControllerPitchInput(MovementVector.Y * -1.0f /** GetWorld()->DeltaTimeSeconds*/);

		//UE_LOG(LogTemp, Warning, TEXT("Input SightAction"));
	}
}

void ATPSCharacter::Fire(/*const FInputActionValue& Value*/)
{
	if (nullptr != CurWeapon)
	{
		CurWeapon->Fire();
		StartCrossHairFire();
	}

	auto AnimInstance = Cast<UTPSAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstance)
	{
		return;
	}

	AnimInstance->PlayFireMontage();
	//UE_LOG(LogTemp, Warning, TEXT("Input FireAction"));
}

void ATPSCharacter::FireStart(const FInputActionValue& Value)
{
	// fire 키가 눌렸을 때
	if (true == Value.Get<bool>())
	{
		// 현재 fire 중이라고 bFiringKey를 true로 설정
		bFiringKey = true;
		// fire 시작 타이머 호출
		FireStartTimer();
	}
}

void ATPSCharacter::FireEnd(const FInputActionValue& Value)
{
	// fire 키가 해제됐을 때
	if (false == Value.Get<bool>())
	{
		// 현재 fire 중이 아니라고 bFiringKey를 false로 설정
		bFiringKey = false;
	}
}

void ATPSCharacter::AimingStart(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		bAiming = true;
	}

}

void ATPSCharacter::AimingEnd(const FInputActionValue& Value)
{
	if (false == Value.Get<bool>())
	{
		bAiming = false;
	}

}

void ATPSCharacter::AimingInterpZoom(float DeltaTime)
{
	if (true == bAiming)
	{
		// 현재 카메라FOV에서 CameraZoomFOV로 Interp
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraZoomFOV, DeltaTime, ZoomInterpSpeed);
	}
	else
	{
		// 현재 카메라FOV에서 CameraFOV로 Interp
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraFOV, DeltaTime, ZoomInterpSpeed);
	}

	GetCamera()->SetFieldOfView(CameraCurrentFOV);
}

void ATPSCharacter::CalculateCrossHairSpread(float DeltaTime)
{
	// Input
	FVector2D WalkSpeedRange = FVector2D(0.0f, 600.0f);
	// Output
	FVector2D VelocityRange = FVector2D(0.0f, 1.0f);
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;

	// Input(WalkSpeedRange)에 대해 Output(VelocityRange) 백분율로 반환(0~600을 0~1로 비율 연산)
	CrossHairVelocity = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityRange, Velocity.Size());

	// 조준상태에 따른 크로스헤어 효과
	if (true == bAiming)
	{
		// 크로스헤어 축소
		CrossHairAim = FMath::FInterpTo(CrossHairAim, 0.5f, DeltaTime, 30.0f);
	}
	else
	{
		// 기존 크로스헤어로 복구
		CrossHairAim = FMath::FInterpTo(CrossHairAim, 0.0f, DeltaTime, 30.0f);
	}

	// SetTimer 지정 시간 이후, 크로스헤어 일시적 풀림 효과
	if (true == bFiring)
	{
		CrossHairFire = FMath::FInterpTo(CrossHairFire, 0.3f, DeltaTime, 60.0f);
	}
	else
	{
		CrossHairFire = FMath::FInterpTo(CrossHairFire, 0.0f, DeltaTime, 60.0f);
	}

	// 움직임에 따른 크로스헤어 최종 결과 연산
	CrossHairSpread = 0.5f + CrossHairVelocity - CrossHairAim + CrossHairFire;
}

void ATPSCharacter::StartCrossHairFire()
{
	bFiring = true;

	// FireTimeDuration뒤 &ATPSCharacter::FinishCrossHairFire 호출
	GetWorldTimerManager().SetTimer(CrossHairFireTimer, this, &ATPSCharacter::FinishCrossHairFire, FireTimeDuration);
}

void ATPSCharacter::FinishCrossHairFire()
{
	bFiring = false;
}

void ATPSCharacter::FireStartTimer()
{
	// 처음 fire가 시작되고 fire가 가능한 상태일 때 true
	if (true == bShouldFire)
	{
		// fire 실행
		Fire();
		// fire를 실행했으므로 fire가 가능한지 체크하는 bShouldFire를 false로 전환
		bShouldFire = false;
		// AutoFireRate뒤 &ATPSCharacter::AutoFireReset 호출
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &ATPSCharacter::AutoFireReset, AutoFireRate);
	}
}

void ATPSCharacter::AutoFireReset()
{
	// 타이머 시간이 지났으므로 다시 fire가 가능하다고 bShouldFire를 true로 전환
	bShouldFire = true;
	// 타이머 시간이 지났고 다시 fire가 가능한 상태에서 fire 키가 작동중이라면
	if (true == bFiringKey)
	{
		// fire 시작 타이머 호출
		FireStartTimer();
	}
	// 타이머 시간이 지났고 다시 fire가 가능한 상태에서 fire 키가 작동중이 아니라면 fire 실행X
}

