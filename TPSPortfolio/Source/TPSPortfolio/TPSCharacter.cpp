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

	GetCapsuleComponent()->SetCollisionProfileName(FName("Character"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CHARACTER(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));
	if (true == SK_CHARACTER.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CHARACTER.Object);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	// 폰 제어 회전 사용
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	// 무브먼트 방향으로 회전 조정
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 입력 매핑
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
		EnhancedInputComponent->BindAction(SightAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Sight);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
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

