// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "TPSItem.h"
#include "TPSWeapon.generated.h"

UCLASS()
class SURVIVEFROMZOMBIES_API ATPSWeapon : public ATPSItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATPSWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void Fire();

	bool GetHitEndLocation(const FVector& MuzzleSocketLocation, FVector& HitEndLocation);
	bool GetCrossHairImpactPoint(FHitResult& OutHitResult, FVector& OutHitLocation);

private:
	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* WeaponMeshComp;

	UPROPERTY(VisibleAnywhere, Category = Sound)
	class USoundCue* FireSound;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* ImpactParticleEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* BeamParticleEffect;
};
