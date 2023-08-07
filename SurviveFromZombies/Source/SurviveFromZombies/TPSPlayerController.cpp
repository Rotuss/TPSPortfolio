// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI_TPSMain.h"

ATPSPlayerController::ATPSPlayerController()
{
}

void ATPSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	const FSoftClassPath MainWidgetClassRef(TEXT("WidgetBlueprint'/Game/HUD/BP_UIMain.BP_UIMain_C'"));
	UClass* TPSMainHUDClass = MainWidgetClassRef.TryLoadClass<UUI_TPSMain>();
	if (nullptr != TPSMainHUDClass)
	{
		MainHUD = CreateWidget<UUI_TPSMain>(this, TPSMainHUDClass);
		if (nullptr != MainHUD)
		{
			MainHUD->AddToViewport();
			MainHUD->SetVisibility(ESlateVisibility::Visible);
		}
	}
}
