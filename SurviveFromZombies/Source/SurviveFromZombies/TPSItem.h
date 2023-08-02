// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SurviveFromZombies.h"
#include "GameFramework/Actor.h"
#include "TPSItem.generated.h"

UCLASS()
class SURVIVEFROMZOMBIES_API ATPSItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATPSItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE class UWidgetComponent* GetPickUpItemWidget() const { return PickUpItemWidget; }

	UPROPERTY(EditAnywhere, Category = Item)
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = Item)
	FString ItemInfo;

protected:
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = Mesh)
	USkeletalMeshComponent* ItemMeshComp;

private:
	UPROPERTY(EditAnywhere, Category = Collision)
	USphereComponent* SphereCollision;
	
	UPROPERTY(EditAnywhere, Category = Collision)
	UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, Category = Widget)
	class UWidgetComponent* PickUpItemWidget;

};
