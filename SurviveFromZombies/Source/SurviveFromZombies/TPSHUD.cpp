// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSHUD.h"
#include "Engine/Canvas.h"

ATPSHUD::ATPSHUD()
{
    CrosshairTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_CROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Cross_Q_9.Cross_Q_9'"));
    if (T_CROSSHAIR.Succeeded())
    {
        CrosshairTexture = T_CROSSHAIR.Object;
    }
}

void ATPSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (nullptr != CrosshairTexture)
    {
        // ȭ�� �߾�
        FVector2D Center = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // �ؽ�ó �߽��� ĵ���� �߽ɿ� �µ��� �ؽ�ó�� ũ�� ���� ��ŭ �������� �ݴϴ�.
        FVector2D CrossHairDrawPosition = FVector2D(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));

        // �߾ӿ� ���ؼ�
        FCanvasTileItem TileItem = FCanvasTileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}
