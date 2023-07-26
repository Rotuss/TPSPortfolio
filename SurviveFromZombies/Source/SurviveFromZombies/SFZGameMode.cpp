// Fill out your copyright notice in the Description page of Project Settings.


#include "SFZGameMode.h"
#include "TPSCharacter.h"
#include "TPSHUD.h"

ASFZGameMode::ASFZGameMode()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
	//PlayerControllerClass = ;
	HUDClass = ATPSHUD::StaticClass();
}
