// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TPSPortfolio.h"
#include "GameFramework/Actor.h"
#include "TPSAmmo.generated.h"

UCLASS()
class TPSPORTFOLIO_API ATPSAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSAmmo();

	//========================================================================
	// 데디케이트 서버 확인용
    // 콜리전 테스트에 사용되는 스피어 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* SphereComponent;

    // 오브젝트의 비주얼 표현을 제공하는 스태틱 메시
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UStaticMeshComponent* StaticMesh;

    // 발사체 움직임을 처리하는 무브먼트 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovementComponent;

    // 발사체가 다른 오브젝트에 영향을 미치고 폭발할 때 사용되는 파티클
    UPROPERTY(EditAnywhere, Category = "Effects")
    class UParticleSystem* ExplosionEffect;

    // 이 발사체가 가할 대미지 타입과 대미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    TSubclassOf<class UDamageType> DamageType;

    // 이 발사체가 가하는 대미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
    float Damage;
	//========================================================================

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //========================================================================
    // 데디케이트 서버 확인용
    virtual void Destroyed() override;

    UFUNCTION(Category = "Projectile")
    void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    //========================================================================

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
