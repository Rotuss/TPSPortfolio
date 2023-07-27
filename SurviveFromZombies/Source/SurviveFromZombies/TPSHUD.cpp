// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSHUD.h"
#include "TPSCharacter.h"
#include "Engine/Canvas.h"

ATPSHUD::ATPSHUD()
    : CrossHairSpread(0.0f)
    , CrossHairSpreadMax(16.0f)
{
    // ��� �ؽ�ó
    CrosshairTopTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRTOPTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_TOPCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Top.Crosshair_Quad_9_Top'"));
    if (T_TOPCROSSHAIR.Succeeded())
    {
        CrosshairTopTexture = T_TOPCROSSHAIR.Object;
    }

    // �ϴ� �ؽ�ó
    CrosshairBottomTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRBOTTOMTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_BOTTOMCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Bottom.Crosshair_Quad_9_Bottom'"));
    if (T_BOTTOMCROSSHAIR.Succeeded())
    {
        CrosshairBottomTexture = T_BOTTOMCROSSHAIR.Object;
    }

    // �´� �ؽ�ó
    CrosshairLeftTexture = CreateDefaultSubobject<UTexture2D>(TEXT("CROSSHAIRLEFTTEXTURE"));
    static ConstructorHelpers::FObjectFinder<UTexture2D> T_LEFTCROSSHAIR(TEXT("Texture2D'/Game/Asset/Texture/CrossHair/Crosshair_Quad_9_Left.Crosshair_Quad_9_Left'"));
    if (T_LEFTCROSSHAIR.Succeeded())
    {
        CrosshairLeftTexture = T_LEFTCROSSHAIR.Object;
    }

    // ��� �ؽ�ó
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
        // ȭ�� �߾�
        FVector2D Center = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        // �ؽ�ó �߽��� ĵ���� �߽ɿ� �µ��� �ؽ�ó�� ũ�� ���� ��ŭ �������� ��
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
            // ���� �÷��� ���� ĳ���͸� ��� ���� �÷��̾� ��Ʈ�ѷ��� ���� ��
            APlayerController* PlayerController = GetOwningPlayerController();
            if (true == IsValid(PlayerController))
            {
                // �÷��̾� ��Ʈ�ѷ��� �̿��Ͽ� ���� ��Ʈ�ѵǰ� �ִ� ĳ���Ͱ� �´��� Cast
                Character = Cast<ATPSCharacter>(PlayerController->GetPawn());
                if (true == IsValid(Character))
                {
                    UE_LOG(LogTemp, Warning, TEXT("TPSHUD Character success"));
                    CrossHairSpread = Character->GetCrossHairSpread();
                }
            }
        }

        // ��� ũ�ν���� ��ġ
        FVector2D CrossHairTopPosition = FVector2D(CrossHairDrawTopPosition.X, CrossHairDrawTopPosition.Y - CrossHairSpread * CrossHairSpreadMax);
        // �ϴ� ũ�ν���� ��ġ
        FVector2D CrossHairBottomPosition = FVector2D(CrossHairDrawBottmoPosition.X, CrossHairDrawBottmoPosition.Y + CrossHairSpread * CrossHairSpreadMax);
        // �´� ũ�ν���� ��ġ
        FVector2D CrossHairLeftPosition = FVector2D(CrossHairDrawLeftPosition.X - CrossHairSpread * CrossHairSpreadMax, CrossHairDrawLeftPosition.Y);
        // ��� ũ�ν���� ��ġ
        FVector2D CrossHairRightPosition = FVector2D(CrossHairDrawRightPosition.X + CrossHairSpread * CrossHairSpreadMax, CrossHairDrawRightPosition.Y);

        // ��� ���ؼ�
        FCanvasTileItem TOPTileItem = FCanvasTileItem(CrossHairTopPosition, CrosshairTopTexture->GetResource(), FLinearColor::White);
        TOPTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TOPTileItem);

        // �ϴ� ���ؼ�
        FCanvasTileItem BottomTileItem = FCanvasTileItem(CrossHairBottomPosition, CrosshairBottomTexture->GetResource(), FLinearColor::White);
        BottomTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(BottomTileItem);

        // �´� ���ؼ�
        FCanvasTileItem LeftTileItem = FCanvasTileItem(CrossHairLeftPosition, CrosshairLeftTexture->GetResource(), FLinearColor::White);
        LeftTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(LeftTileItem);

        // ��� ���ؼ�
        FCanvasTileItem RightTileItem = FCanvasTileItem(CrossHairRightPosition, CrosshairRightTexture->GetResource(), FLinearColor::White);
        RightTileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(RightTileItem);
    }
}

void ATPSHUD::BeginPlay()
{
    Super::BeginPlay();

    // ���� �÷��� ���� ĳ���͸� ��� ���� �÷��̾� ��Ʈ�ѷ��� ���� ��
    APlayerController* PlayerController = GetOwningPlayerController();
    if (true == IsValid(PlayerController))
    {
        // �÷��̾� ��Ʈ�ѷ��� �̿��Ͽ� ���� ��Ʈ�ѵǰ� �ִ� ĳ���Ͱ� �´��� Cast
        Character = Cast<ATPSCharacter>(PlayerController->GetPawn());
        if (false == IsValid(Character))
        {
            UE_LOG(LogTemp, Error, TEXT("TPSHUD BeginPlay Character is nullptr!"));
        }
    }
}
