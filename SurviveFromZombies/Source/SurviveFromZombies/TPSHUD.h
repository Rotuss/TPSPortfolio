// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "GameFramework/HUD.h"
#include "TPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEFROMZOMBIES_API ATPSHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ATPSHUD();
	
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

private:

};
