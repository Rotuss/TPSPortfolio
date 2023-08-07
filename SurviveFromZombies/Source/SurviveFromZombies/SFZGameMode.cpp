// Fill out your copyright notice in the Description page of Project Settings.


#include "SFZGameMode.h"
#include "TPSCharacter.h"
#include "TPSPlayerController.h"
#include "TPSHUD.h"

ASFZGameMode::ASFZGameMode()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSHUD::StaticClass();
}
