// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSHUD.h"
#include "TPSCharacter.h"
#include "Engine/Canvas.h"

ATPSHUD::ATPSHUD()
    : CrossHairSpread(0.0f)
    , CrossHairSpreadMax(16.0f)
{
    // 상단 텍스처
    CrosshairTopTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRTOPTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_TOPCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Top.Crosshair_Quad_9_Top'"));
    if (T_TOPCROSSHAIR.Succeeded())
    {
        CrosshairTopTexture = T_TOPCROSSHAIR.Object;
    }

    // 하단 텍스처
    CrosshairBottomTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRBOTTOMTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_BOTTOMCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Bottom.Crosshair_Quad_9_Bottom'"));
    if (T_BOTTOMCROSSHAIR.Succeeded())
    {
        CrosshairBottomTexture = T_BOTTOMCROSSHAIR.Object;
    }

    // 좌단 텍스처
    CrosshairLeftTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRLEFTTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_LEFTCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Left.Crosshair_Quad_9_Left'"));
    if (T_LEFTCROSSHAIR.Succeeded())
    {
        CrosshairLeftTexture = T_LEFTCROSSHAIR.Object;
    }

    // 우단 텍스처
    CrosshairRightTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRRIGHTTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_RIGHTCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Right.Crosshair_Quad_9_Right'"));
    if (T_RIGHTCROSSHAIR.Succeeded())
    {
        CrosshairRightTexture = T_RIGHTCROSSHAIR.Object;
    }
}

void ATPSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (nullptr != CrosshairTopTexture && nullptr != CrosshairBottomTexture
        && nullptr != CrosshairLeftTexture && nullptr != CrosshairRightTexture)
    {
        // 화면 중앙
        FVector2D Center = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // 텍스처 중심이 캔버스 중심에 맞도록 텍스처의 크기 절반 만큼 오프셋을 줌
        FVector2D CrossHairDrawTopPosition = FVector2D(Center.X - (CrosshairTopTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTopTexture->GetSurfaceHeight() * 0.5f));
        FVector2D CrossHairDrawBottmoPosition = FVector2D(Center.X - (CrosshairBottomTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairBottomTexture->GetSurfaceHeight() * 0.5f));
        FVector2D CrossHairDrawLeftPosition = FVector2D(Center.X - (CrosshairLeftTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairLeftTexture->GetSurfaceHeight() * 0.5f));
        FVector2D CrossHairDrawRightPosition = FVector2D(Center.X - (CrosshairRightTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairRightTexture->GetSurfaceHeight() * 0.5f));

        if (true == IsValid(Character))
        {
            CrossHairSpread = Character->GetCrossHairSpread();
        }
        else
        {
            // 현재 플레이 중인 캐릭터를 얻기 위해 플레이어 컨트롤러를 가져 옴
            APlayerController* PlayerController = GetOwningPlayerController();
            if (true == IsValid(PlayerController))
            {
                // 플레이어 컨트롤러를 이용하여 현재 컨트롤되고 있는 캐릭터가 맞는지 Cast
                Character = Cast<ATPSCharacter>(PlayerController->GetPawn());
                if (true == IsValid(Character))
                {
                    UE_LOG(LogTemp, Warning, TEXT("TPSHUD Character success"));
                    CrossHairSpread = Character->GetCrossHairSpread();
                }
            }
        }

        // 상단 크로스헤어 위치
        FVector2D CrossHairTopPosition = FVector2D(CrossHairDrawTopPosition.X, CrossHairDrawTopPosition.Y - CrossHairSpread * CrossHairSpreadMax);
        // 하단 크로스헤어 위치
        FVector2D CrossHairBottomPosition = FVector2D(CrossHairDrawBottmoPosition.X, CrossHairDrawBottmoPosition.Y + CrossHairSpread * CrossHairSpreadMax);
        // 좌단 크로스헤어 위치
        FVector2D CrossHairLeftPosition = FVector2D(CrossHairDrawLeftPosition.X - CrossHairSpread * CrossHairSpreadMax, CrossHairDrawLeftPosition.Y);
        // 우단 크로스헤어 위치
        FVector2D CrossHairRightPosition = FVector2D(CrossHairDrawRightPosition.X + CrossHairSpread * CrossHairSpreadMax, CrossHairDrawRightPosition.Y);

        // 상단 조준선
        FCanvasTileItem TOPTileItem = FCanvasTileItem(CrossHairTopPosition, CrosshairTopTexture->GetResource(), FLinearColor::White);
        TOPTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TOPTileItem);

        // 하단 조준선
        FCanvasTileItem BottomTileItem = FCanvasTileItem(CrossHairBottomPosition, CrosshairBottomTexture->GetResource(), FLinearColor::White);
        BottomTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(BottomTileItem);

        // 좌단 조준선
        FCanvasTileItem LeftTileItem = FCanvasTileItem(CrossHairLeftPosition, CrosshairLeftTexture->GetResource(), FLinearColor::White);
        LeftTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(LeftTileItem);

        // 우단 조준선
        FCanvasTileItem RightTileItem = FCanvasTileItem(CrossHairRightPosition, CrosshairRightTexture->GetResource(), FLinearColor::White);
        RightTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(RightTileItem);
    }
}

void ATPSHUD::BeginPlay()
{
    Super::BeginPlay();

    // 현재 플레이 중인 캐릭터를 얻기 위해 플레이어 컨트롤러를 가져 옴
    APlayerController* PlayerController = GetOwningPlayerController();
    if (true == IsValid(PlayerController))
    {
        // 플레이어 컨트롤러를 이용하여 현재 컨트롤되고 있는 캐릭터가 맞는지 Cast
        Character = Cast<ATPSCharacter>(PlayerController->GetPawn());
        if (false == IsValid(Character))
        {
            UE_LOG(LogTemp, Error, TEXT("TPSHUD BeginPlay Character is nullptr!"));
        }
    }
}
