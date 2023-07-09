// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPortfolio.h"
#include "Animation/AnimInstance.h"
#include "TPSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPSPORTFOLIO_API UTPSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTPSAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void SetSpeed(float InputSpeed)
	{
		Speed = InputSpeed;
	}

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	FVector Velocity;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = true))
	bool CanMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = true))
	bool IsJumping;
};
