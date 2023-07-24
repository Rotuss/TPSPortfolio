// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"
#include "TPSCharacter.h"

UTPSAnimInstance::UTPSAnimInstance()
{
	Velocity = FVector::ZeroVector;
	Speed = 0.0f;
	IsMoving = false;
	IsJumping = false;
	
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
