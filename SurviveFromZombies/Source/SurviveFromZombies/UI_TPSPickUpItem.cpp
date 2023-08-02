// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TPSPickUpItem.h"
#include "TPSItem.h"

void UUI_TPSPickUpItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UUI_TPSPickUpItem::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (true == IsValid(TPSItemRef))
	{
		ItemNameText->SetText(FText::FromString(TPSItemRef->ItemName));
		ItemInfoText->SetText(FText::FromString(TPSItemRef->ItemInfo));
	}
}
