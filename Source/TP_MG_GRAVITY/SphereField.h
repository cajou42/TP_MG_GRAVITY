// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MG_Player.h"
#include "SphereField.generated.h"

UCLASS()
class TP_MG_GRAVITY_API ASphereField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASphereField();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Field;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FieldForce = 150.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AMG_Player* PlayerRef = nullptr;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
