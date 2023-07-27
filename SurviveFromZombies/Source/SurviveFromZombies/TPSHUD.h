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
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTopTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairBottomTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairLeftTexture;

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairRightTexture;

private:
	class ATPSCharacter* Character;

	float CrossHairSpread;
	float CrossHairSpreadMax;
};
