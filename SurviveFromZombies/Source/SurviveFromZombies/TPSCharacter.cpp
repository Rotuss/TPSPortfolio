// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Character/TPS/Ch15_nonPBR.Ch15_nonPBR'"));
	if (true == SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	// �Է�
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

}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		EnhancedInputComponent->BindAction(SightAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Sight);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	}

}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FRotator ControllerRotation = Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, ControllerRotation.Yaw, 0.0f);
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// �յ�(W,S)
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), MovementVector.Y /** GetWorld()->DeltaTimeSeconds*/);
		// �¿�(D,A)
		AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), MovementVector.X /** GetWorld()->DeltaTimeSeconds*/);

		//UE_LOG(LogTemp, Warning, TEXT("Input MoveAction"));
	}
}

void ATPSCharacter::Sight(const FInputActionValue& Value)
{
	if (true == Value.Get<bool>())
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		// �¿�
		AddControllerYawInput(MovementVector.X /** GetWorld()->DeltaTimeSeconds*/);
		// ����
		AddControllerPitchInput(MovementVector.Y * -1.0f /** GetWorld()->DeltaTimeSeconds*/);

		//UE_LOG(LogTemp, Warning, TEXT("Input SightAction"));
	}
}
