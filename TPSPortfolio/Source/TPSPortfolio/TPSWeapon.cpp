// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
//#include "Kismet/GameplayStatics.h"

// Sets default values
ATPSWeapon::ATPSWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPONMESH"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (SK_WEAPON.Succeeded())
	{
		WeaponMeshComp->SetSkeletalMesh(SK_WEAPON.Object);
	}
	WeaponMeshComp->SetCollisionProfileName(FName("Weapon"));
	RootComponent = WeaponMeshComp;

	//MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("MUZZLEFLASHEFFECT"));
	//static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPSWeapon::PullTrigger()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon Trigger Check"));
	
	//UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, WeaponMeshComp, TEXT("b_gun_muzzleflash"));
}

