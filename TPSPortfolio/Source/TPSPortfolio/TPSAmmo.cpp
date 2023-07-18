// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAmmo.h"

// ����
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATPSAmmo::ATPSAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//========================================================================
	// ��������Ʈ ���� Ȯ�ο�
	bReplicates = true;

	// �߻�ü�� �ݸ����� ��Ʈ ������Ʈ ������ �� SphereComponent ����
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->InitSphereRadius(3.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	// �߻�ü �浹 �Լ��� ��Ʈ �̺�Ʈ�� ���
	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &ATPSAmmo::OnProjectileImpact);
	}

	// ���־� ǥ���� ����� �޽� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultMesh(TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/Ammunition/SM_Shell_762x39.SM_Shell_762x39'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	// ����� �޽� ������ �߰ߵǸ� ����ƽ �޽ÿ� ��ġ/������ ����
	if (true == DefaultMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(DefaultMesh.Object);
		StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -3.0f));
		StaticMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		StaticMesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 5.0f));
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> DefaultExplosionEffect(TEXT("ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Default.P_Default'"));
	if (true == DefaultExplosionEffect.Succeeded())
	{
		ExplosionEffect = DefaultExplosionEffect.Object;
	}

	// �߻�ü �����Ʈ ������Ʈ ����
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("AmmoMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;
	//========================================================================
}

// Called when the game starts or when spawned
void ATPSAmmo::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPSAmmo::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, spawnLocation, GetActorRotation() + FRotator(90.0, 0.0, 0.0), true, EPSCPoolMethod::AutoRelease);
}

void ATPSAmmo::OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (nullptr != OtherActor)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, NormalImpulse, Hit, GetInstigator()->Controller, this, DamageType);
	}

	Destroy();
}

// Called every frame
void ATPSAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

