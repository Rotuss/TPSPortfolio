// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEFROMZOMBIES_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATPSPlayerController();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE UUserWidget* GetMainHUD() const { return MainHUD; }

private:
	UPROPERTY(EditAnywhere, Category = Widget)
	UUserWidget* MainHUD;
};
