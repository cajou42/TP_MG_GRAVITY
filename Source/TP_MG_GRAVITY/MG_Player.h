// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MG_Player.generated.h"
UENUM(BlueprintType)
enum class GravityDirection : uint8 {
	GD_Plane = 0,
	GD_Sphere = 1,
	GD_Cube = 2,
	GD_Cylinder = 3,
	GD_Torus = 4
};

UCLASS()
class TP_MG_GRAVITY_API AMG_Player : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMG_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity = 981.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsGrounded = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpVelocity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult PlayerHit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FallingDirection = GetActorUpVector() * -75;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GravityDirection Direction = GravityDirection::GD_Plane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlanetAttraction = FVector::ZeroVector;
};
