// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Sound/SoundCue.h"

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

	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("MUZZLEFLASHEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_WEAPONEMITTER(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
	if (P_WEAPONEMITTER.Succeeded())
	{
		MuzzleFlashEffect = P_WEAPONEMITTER.Object;
	}

	FireSound = CreateDefaultSubobject<USoundCue>(TEXT("FIRESOUND"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_FIRESOUND(TEXT("SoundCue'/Game/Asset/Sound/Shot/AR_Shot_Cue.AR_Shot_Cue'"));
	if (SC_FIRESOUND.Succeeded())
	{
		FireSound = SC_FIRESOUND.Object;
	}

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

void ATPSWeapon::Fire()
{
	if (nullptr != FireSound)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, WeaponMeshComp, TEXT("b_gun_muzzleflash"));
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

