// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MG_Player.h"
#include "TorusField.generated.h"

UCLASS()
class TP_MG_GRAVITY_API ATorusField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATorusField();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* OutsideField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* InsideField;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FieldForce = 150.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Planet;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	AMG_Player* PlayerRef = nullptr;
	bool ChangeGravity = false;
	FHitResult* CurrentHit;
	FVector Gu;
	FVector Gr;
	float Rayon;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
