// Fill out your copyright notice in the Description page of Project Settings.


#include "Planefield.h"


// Sets default values
APlanefield::APlanefield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Collision Box
	Field = CreateDefaultSubobject<UBoxComponent>(FName("Collision Zone"));
	Field->SetRelativeScale3D(FVector(2, 2, 2));
	Field->SetVisibility(true);
	Field->SetHiddenInGame(false);
	Field->SetVisibleInSceneCaptureOnly(false);

	// Add event to the OnComponentBeginOverlap delegate
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	Field->SetupAttachment(RootComponent);
	Field->OnComponentBeginOverlap.AddDynamic(this, &APlanefield::OnOverlapBegin);
	Field->OnComponentEndOverlap.AddDynamic(this, &APlanefield::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void APlanefield::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlanefield::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		ChangeGravity = true;
	}
}

void APlanefield::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		ChangeGravity = false;
		PlayerRef->IsGrounded = false;
	}
}

// Called every frame
void APlanefield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChangeGravity)
	{
		PlayerRef->FallingDirection = GetActorUpVector() * 75;
		PlayerRef->AddActorWorldOffset(GetActorUpVector() * FieldForce * DeltaTime, true);
		//PlayerRef->SetActorRotation(FRotator(0,GetActorRotation().Pitch + 180,0));
	}
}

