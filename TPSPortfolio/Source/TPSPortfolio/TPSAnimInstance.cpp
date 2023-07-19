// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAnimInstance.h"

UTPSAnimInstance::UTPSAnimInstance()
{
    Velocity = FVector::ZeroVector;
    Speed = 0.0f;
    CanMove = false;
    IsJumping = false;
}

void UTPSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto Pawn = TryGetPawnOwner();
    if (true == IsValid(Pawn))
    {
        Speed = Pawn->GetVelocity().Size2D();

        auto Character = Cast<ACharacter>(Pawn);
        if (true == IsValid(Character))
        {
            Velocity = Character->GetMovementComponent()->Velocity;
            //Speed = Velocity.Size2D();
            CanMove = Speed > 0.0f ? true : false;
            IsJumping = Character->GetMovementComponent()->IsFalling();
        }
    }

}
