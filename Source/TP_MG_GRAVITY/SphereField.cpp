// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereField.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASphereField::ASphereField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Field = CreateDefaultSubobject<USphereComponent>(FName("Collision Zone"));
	Field->SetRelativeScale3D(FVector(2, 2, 2));
	Field->SetVisibility(true);
	Field->SetHiddenInGame(false);
	Field->SetVisibleInSceneCaptureOnly(false);

	// Add event to the OnComponentBeginOverlap delegate
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	Field->SetupAttachment(RootComponent);
	Field->OnComponentBeginOverlap.AddDynamic(this, &ASphereField::OnOverlapBegin);
	Field->OnComponentEndOverlap.AddDynamic(this, &ASphereField::OnComponentEndOverlap);
}

// Called when the game starts or when spawned
void ASphereField::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASphereField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		PlayerRef->Direction = GravityDirection::GD_Sphere;
	}
}

void ASphereField::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		PlayerRef->Direction = GravityDirection::GD_Plane;
		PlayerRef->IsGrounded = false;
	}
}

// Called every frame
void ASphereField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerRef && PlayerRef->Direction == GravityDirection::GD_Sphere)
	{
		FVector Attraction = GetActorLocation() - PlayerRef->GetActorLocation();
		float DistanceToCenter = Attraction.Size();
		FVector NormalizedDirection = Attraction / DistanceToCenter;
		
		PlayerRef->AddActorWorldOffset( Attraction * DeltaTime, true);
		PlayerRef->PlanetAttraction = Attraction.GetSafeNormal();

		
		//FRotator TargetRotation = NormalizedDirection.Rotation();
		//FRotator NewRotation = FMath::RInterpTo(PlayerRef->GetActorRotation(), TargetRotation, DeltaTime, 40.f);
		//PlayerRef->SetActorRotation(NewRotation);
	}
}

