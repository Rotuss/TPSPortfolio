// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UI_TPSMain.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEFROMZOMBIES_API UUI_TPSMain : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& _geo, float _DeltaTime);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxAmmo;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	class ATPSCharacter* TPSCharacterRef;
};
