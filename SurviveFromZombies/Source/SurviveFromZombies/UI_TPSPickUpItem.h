// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UI_TPSPickUpItem.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVEFROMZOMBIES_API UUI_TPSPickUpItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemInfoText;

	class ATPSItem* TPSItemRef;
};
