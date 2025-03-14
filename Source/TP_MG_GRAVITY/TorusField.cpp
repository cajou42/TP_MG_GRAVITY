// Fill out your copyright notice in the Description page of Project Settings.


#include "TorusField.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATorusField::ATorusField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Game/Content/Torus"));
	UStaticMesh* Asset = MeshAsset.Object;
	
	OutsideField = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshOutsideCollider"));
	OutsideField->SetStaticMesh(Asset);
	OutsideField->SetRelativeScale3D(FVector(1, 1, 1));
	
	InsideField = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshInsideCollider"));
	InsideField->SetStaticMesh(Asset);
	InsideField->SetRelativeScale3D(FVector(0.25, 0.25, 0.25));
	
	Planet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	Planet->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	Planet->SetStaticMesh(Asset);

	OutsideField->SetupAttachment(RootComponent);
	InsideField->SetupAttachment(RootComponent);
	Planet->SetupAttachment(RootComponent);
	// Add event to the OnComponentBeginOverlap delegate
	OutsideField->OnComponentBeginOverlap.AddDynamic(this, &ATorusField::OnOverlapBegin);
	OutsideField->OnComponentEndOverlap.AddDynamic(this, &ATorusField::OnComponentEndOverlap);
	InsideField->OnComponentBeginOverlap.AddDynamic(this, &ATorusField::OnOverlapBegin);
	InsideField->OnComponentEndOverlap.AddDynamic(this, &ATorusField::OnComponentEndOverlap);

	Gu = GetActorUpVector();
	Gr = GetActorRightVector();
}

// Called when the game starts or when spawned
void ATorusField::BeginPlay()
{
	Super::BeginPlay();
	Rayon = (Planet->GetComponentScale().Y * 90) / 2;
}

void ATorusField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		ChangeGravity = true;
	}
}

void ATorusField::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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
void ATorusField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChangeGravity)
	{
		FVector PlayerRelativePos = PlayerRef->GetActorLocation() - GetActorLocation();

		// Project player on tore
		FVector ProjectedPos = PlayerRelativePos - (PlayerRelativePos.Dot(Gu) * Gu);
		ProjectedPos.Normalize();

		// Calculate the closest point on the torus from the player
		FVector ClosestPointOnRing = GetActorLocation() + ProjectedPos * Rayon;

		// Make player attraction toward this point
		FVector Attraction = ClosestPointOnRing - PlayerRef->GetActorLocation();
		Attraction.Normalize();

		PlayerRef->AddActorWorldOffset(Attraction * FieldForce * DeltaTime, true);

		//FRotator rotate = UKismetMathLibrary::MakeRotFromZ(-Attraction);
		//FRotator NewRotation = FMath::RInterpTo(PlayerRef->GetActorRotation(),rotate,DeltaTime,40.f);
		//PlayerRef->SetActorRotation(NewRotation);

		// Debug
		DrawDebugLine(GetWorld(), PlayerRef->GetActorLocation(), ClosestPointOnRing, FColor::Green);
	}

}

