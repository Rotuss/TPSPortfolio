// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_TPSMain.h"
#include "TPSCharacter.h"
#include "TPSWeapon.h"

void UUI_TPSMain::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TPSCharacterRef = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

}

void UUI_TPSMain::NativeConstruct()
{
	Super::NativeConstruct();

	/*if (true == IsValid(TPSCharacterRef))
	{		
		if (nullptr != TPSCharacterRef->GetCurrentWeapon())
		{
			FString maxtmp = FString::FromInt(TPSCharacterRef->GetCurrentWeapon()->GetMaxAmmo());
			FString tmp = FString::FromInt(TPSCharacterRef->GetCurrentWeapon()->GetAmmo());
			MaxAmmo->SetText(FText::FromString(maxtmp));
			CurrentAmmo->SetText(FText::FromString(tmp));
		}
	}*/
}

void UUI_TPSMain::NativeTick(const FGeometry& _geo, float _DeltaTime)
{
	Super::NativeTick(_geo, _DeltaTime);
	
	// ... 위험요소 충분...
	if (nullptr != TPSCharacterRef->GetCurrentWeapon())
	{
		FString MaxAmmoTxt= FString::FromInt(TPSCharacterRef->GetCurrentWeapon()->GetMaxAmmo());
		FString CurAmmoTxt = FString::FromInt(TPSCharacterRef->GetCurrentWeapon()->GetAmmo());
		MaxAmmo->SetText(FText::FromString(MaxAmmoTxt));
		CurrentAmmo->SetText(FText::FromString(CurAmmoTxt));
	}
}
