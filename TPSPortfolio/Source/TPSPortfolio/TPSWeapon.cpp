// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TPSAmmo.h"

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

	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("MUZZLEFLASHEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_WEAPONEMITTER(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
	if (P_WEAPONEMITTER.Succeeded())
	{
		MuzzleFlashEffect = P_WEAPONEMITTER.Object;
	}

	WeaponHitEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("WEAPONHITEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITMITTER(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Blood/P_Blood_Splat_Cone.P_Blood_Splat_Cone'"));
	if (P_HITMITTER.Succeeded())
	{
		WeaponHitEffect = P_HITMITTER.Object;
	}

	MaxRange = 1000.0f;
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
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, WeaponMeshComp, TEXT("b_gun_muzzleflash"));
	
	// 라인트레이스 총구 방향? 뷰포트 방향? -> 우선 총구(위치) 및 플레이어(회전) 방향
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (nullptr == OwnerPawn)
	{
		return;
	}

	FVector Location = WeaponMeshComp->GetSocketLocation(TEXT("b_gun_muzzleflash"));
	FRotator Rotation = OwnerPawn->GetActorRotation();
	FVector End = Location + Rotation.Vector() * MaxRange;

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = OwnerPawn->GetInstigator();
	spawnParameters.Owner = OwnerPawn;

	ATPSAmmo* spawnedProjectile = GetWorld()->SpawnActor<ATPSAmmo>(Location + Location.ForwardVector * 50.0f, End.Rotation(), spawnParameters);

	//FHitResult Hit;
	//bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel2);
	//if (true == bSuccess)
	//{
	//	//DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
	//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponHitEffect, Hit.ImpactPoint, Rotation + FRotator(90.0, 0.0, 0.0));
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Weapon Trigger Check"));
}

