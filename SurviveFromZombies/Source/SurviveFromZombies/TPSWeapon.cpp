// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSWeapon.h"
#include "DrawDebugHelpers.h"
#include "Sound/SoundCue.h"

// Sets default values
ATPSWeapon::ATPSWeapon()
	: MaxRifleAmmo(100)
	, CurrentRifleAmmo()
	, Ammo()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPONMESH"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	if (SK_WEAPON.Succeeded())
	{
		ItemMeshComp->SetSkeletalMesh(SK_WEAPON.Object);
	}

	MuzzleFlashEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("MUZZLEFLASHEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_WEAPONEMITTER(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
	if (P_WEAPONEMITTER.Succeeded())
	{
		MuzzleFlashEffect = P_WEAPONEMITTER.Object;
	}

	/*ImpactParticleEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("IMPACTPARTICLEEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITMITTER(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
	if (P_HITMITTER.Succeeded())
	{
		ImpactParticleEffect = P_HITMITTER.Object;
	}*/

	BeamParticleEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("IMPACTPARTICLEEFFECT"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_SMOKEMITTER(TEXT("ParticleSystem'/Game/Asset/FX/Smoke/P_SmokeTrail_Faded.P_SmokeTrail_Faded'"));
	if (P_SMOKEMITTER.Succeeded())
	{
		BeamParticleEffect = P_SMOKEMITTER.Object;
	}

	FireSound = CreateDefaultSubobject<USoundCue>(TEXT("FIRESOUND"));
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_FIRESOUND(TEXT("SoundCue'/Game/Asset/Sound/Shot/AR_Shot_Cue.AR_Shot_Cue'"));
	if (SC_FIRESOUND.Succeeded())
	{
		FireSound = SC_FIRESOUND.Object;
	}

	// 임시로 MaxAmmo와 CurrentAmmo를 확인하기 위한 설정
	Ammo = MaxRifleAmmo;
}

// Called when the game starts or when spawned
void ATPSWeapon::BeginPlay()
{
	Super::BeginPlay();

	// Ammo 수량 초기화
	InitMapAmmo();
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
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, ItemMeshComp, TEXT("b_gun_muzzleflash"));
		UGameplayStatics::PlaySound2D(this, FireSound);

		//FHitResult FireHit;
		//const FVector Start = WeaponMeshComp->GetSocketLocation(TEXT("MuzzleflashSocket"));
		//const FRotator Rotation = WeaponMeshComp->GetSocketRotation(TEXT("MuzzleflashSocket"));
		//const FVector End = Start + Rotation.Vector() * 50000.0f;

		//// 부딪혔을 때 End 위치 조정을 위해 분리
		//FVector BeamEndPoint = End;

		//GetWorld()->LineTraceSingleByChannel(FireHit, Start, End, ECollisionChannel::ECC_Visibility);
		//if (true == FireHit.bBlockingHit)
		//{
		//	// 디버그 확인용
		//	/*DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);
		//	DrawDebugPoint(GetWorld(), FireHit.Location, 5.0f, FColor::Blue, false, 2.0f);*/

		//	// 물체에 부딪혔을 때 FireHit 지점을 End로 재설정
		//	BeamEndPoint = FireHit.ImpactPoint;

		//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, FireHit.ImpactPoint, Rotation + FRotator(90.0, 0.0, 0.0));
		//}

		//UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticleEffect, WeaponMeshComp->GetSocketTransform(TEXT("MuzzleflashSocket")));
		//Beam->SetVectorParameter(TEXT("Target"), BeamEndPoint);

		FVector HitEnd;
		bool bHitEnd = GetHitEndLocation(ItemMeshComp->GetSocketLocation(TEXT("MuzzleflashSocket")), HitEnd);
		if (true == bHitEnd)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, HitEnd, HitEnd.Rotation() + FRotator(90.0, 0.0, 0.0));
		}

		UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BeamParticleEffect, ItemMeshComp->GetSocketTransform(TEXT("MuzzleflashSocket")));
		Beam->SetVectorParameter(TEXT("Target"), HitEnd);
	}

	// Ammo가 0보다 크면 Ammo 감소 0이하면 0
	Ammo = 0 < Ammo ? Ammo - 1 : 0;
}

bool ATPSWeapon::GetHitEndLocation(const FVector& MuzzleSocketLocation, FVector& HitEndLocation)
{
	// 크로스헤어와 충돌하는지 체크
	FHitResult CrossHairHitResult;
	bool bCrossHairHit = GetCrossHairImpactPoint(CrossHairHitResult, HitEndLocation);

	if (true == bCrossHairHit)
	{
		HitEndLocation = CrossHairHitResult.ImpactPoint;
	}

	// 총구와 크로스헤어 사이 물체가 있을 경우 물체에 히트
	FHitResult WeaponTraceHit;
	const FVector WeaponTraceStart = MuzzleSocketLocation;
	const FVector StartToEnd = HitEndLocation - WeaponTraceStart;
	const FVector WeaponTraceEnd = MuzzleSocketLocation + StartToEnd * 1.25f;

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
	if (true == WeaponTraceHit.bBlockingHit)
	{
		HitEndLocation = WeaponTraceHit.ImpactPoint;

		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *WeaponTraceHit.GetActor()->GetName());
		
		return true;
	}

	//FVector2D ViewportSize;
	//if (nullptr != GEngine && GEngine->GameViewport)
	//{
	//	GEngine->GameViewport->GetViewportSize(ViewportSize);
	//}

	//// 크로스헤어 스크린 위치, 방향 잡기
	//FVector2D CrossHairLocation = FVector2D(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	//FVector CrossHairWorldPosition;
	//FVector CrossHairWorldDirection;

	//// 2D 화면 공간 좌표(크로스헤어 스크린)를 3D 월드 공간 포지션과 방향으로 변환
	//bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
	//	CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection);

	//if (true == bScreenToWorld)
	//{
	//	FHitResult ScreenTraceHit;
	//	const FVector Start = CrossHairWorldPosition;
	//	const FVector End = CrossHairWorldPosition + CrossHairWorldDirection * 50000.0f;
	//	HitEndLocation = End;

	//	GetWorld()->LineTraceSingleByChannel(ScreenTraceHit, Start, End, ECollisionChannel::ECC_Visibility);
	//	if (true == ScreenTraceHit.bBlockingHit)
	//	{
	//		HitEndLocation = ScreenTraceHit.ImpactPoint;
	//	}

	//	// 총구와 크로스헤어 사이 물체가 있을 경우 물체에 히트
	//	FHitResult WeaponTraceHit;
	//	const FVector WeaponTraceStart = MuzzleSocketLocation;
	//	const FVector WeaponTraceEnd = HitEndLocation;

	//	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility);
	//	if (true == WeaponTraceHit.bBlockingHit)
	//	{
	//		HitEndLocation = WeaponTraceHit.ImpactPoint;
	//	}

	//	return true;
	//}

	return false;
}

bool ATPSWeapon::GetCrossHairImpactPoint(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if (nullptr != GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// 크로스헤어 스크린 위치, 방향 잡기
	FVector2D CrossHairLocation = FVector2D(ViewportSize.X * 0.5f, ViewportSize.Y * 0.5f);
	FVector CrossHairWorldPosition;
	FVector CrossHairWorldDirection;

	// 2D 화면 공간 좌표(크로스헤어 스크린)를 3D 월드 공간 포지션과 방향으로 변환
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrossHairLocation, CrossHairWorldPosition, CrossHairWorldDirection);
	if (true == bScreenToWorld)
	{
		const FVector Start = CrossHairWorldPosition;
		const FVector End = CrossHairWorldPosition + CrossHairWorldDirection * 50000.0f;
		OutHitLocation = End;

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
		if (true == OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.ImpactPoint;

			return true;
		}
	}

	return false;
}

void ATPSWeapon::InitMapAmmo()
{
	MapAmmo.Add(EAmmoType::AR, MaxRifleAmmo);
}

