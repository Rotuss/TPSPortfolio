// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameMode.h"
#include "TPSCharacter.h"
#include "TPSPlayerController.h"

ATPSGameMode::ATPSGameMode()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
}
