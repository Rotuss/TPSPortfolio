// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPortfolio.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPSPORTFOLIO_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void BeginPlay() override;
};
