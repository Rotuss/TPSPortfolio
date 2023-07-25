// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"
#include "TPSCharacter.h"

UTPSAnimInstance::UTPSAnimInstance()
{
	Velocity = FVector::ZeroVector;
	Speed = 0.0f;
	IsMoving = false;
	IsJumping = false;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FIRE_MONTAGE(TEXT("AnimMontage'/Game/Character/Animation/FireMontage.FireMontage'"));
	if (FIRE_MONTAGE.Succeeded())
	{
		FireMontage = FIRE_MONTAGE.Object;
	}

}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto Pawn = TryGetPawnOwner();

	if (true == IsValid(Pawn))
	{
		//Speed = Pawn->GetVelocity().Size2D();

		auto Character = Cast<ATPSCharacter>(Pawn);
		if (true == IsValid(Character))
		{
			Velocity = Character->GetMovementComponent()->Velocity;
			Speed = Velocity.Size2D();
			IsMoving = Speed > 0.0f ? true : false;
			IsJumping = Character->GetMovementComponent()->IsFalling();
		}
	}
	
}

void UTPSAnimInstance::PlayFireMontage()
{
	if (false == Montage_IsPlaying(FireMontage))
	{
		Montage_Play(FireMontage);
	}
}
