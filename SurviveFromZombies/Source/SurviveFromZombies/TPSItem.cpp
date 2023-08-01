// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSItem.h"
#include "TPSCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values
ATPSItem::ATPSItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ITEMMESH"));
	RootComponent = ItemMeshComp;

	// 범위내 접근 확인 콜리전
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERECOLLISION"));
	SphereCollision->SetupAttachment(ItemMeshComp);
	SphereCollision->SetSphereRadius(150.0f);
	SphereCollision->bHiddenInGame = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCOLLISION"));
	BoxCollision->SetupAttachment(ItemMeshComp);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	PickUpItemWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PICKUPITEMWIDGET"));
	PickUpItemWidget->SetupAttachment(ItemMeshComp);
	PickUpItemWidget->SetRelativeLocation(FVector(0.0f, 80.0f, 50.0f));
	PickUpItemWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_PICKUPITEM(TEXT("WidgetBlueprint'/Game/HUD/BP_UIPickUpItem.BP_UIPickUpItem_C'"));
	if (UI_PICKUPITEM.Succeeded())
	{
		PickUpItemWidget->SetWidgetClass(UI_PICKUPITEM.Class);
		PickUpItemWidget->SetDrawSize(FVector2D(400.0f, 150.0f));
	}
}

// Called when the game starts or when spawned
void ATPSItem::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATPSItem::OnSphereOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ATPSItem::OnSphereEndOverlap);

	PickUpItemWidget->SetVisibility(false);
}

// Called every frame
void ATPSItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (nullptr != OtherActor)
	{
		ATPSCharacter* Character = Cast<ATPSCharacter>(OtherActor);
		if (nullptr != Character)
		{
			Character->IncrementOverlappedItemCount(1);
		}
	}
}

void ATPSItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (nullptr != OtherActor)
	{
		ATPSCharacter* Character = Cast<ATPSCharacter>(OtherActor);
		if (nullptr != Character)
		{
			Character->IncrementOverlappedItemCount(-1);
		}
	}
}

