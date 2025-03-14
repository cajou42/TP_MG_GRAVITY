// Fill out your copyright notice in the Description page of Project Settings.


#include "CubicalField.h"

#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACubicalField::ACubicalField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Collision Box
	Field = CreateDefaultSubobject<UBoxComponent>(FName("Collision Zone"));
	Field->SetRelativeScale3D(FVector(2, 2, 2));
	Field->SetVisibility(true);
	Field->SetHiddenInGame(false);
	Field->SetVisibleInSceneCaptureOnly(false);

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	UStaticMesh* Asset = MeshAsset.Object;
	MeshCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	MeshCenter->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	MeshCenter->SetStaticMesh(Asset);

	// Add event to the OnComponentBeginOverlap delegate
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	MeshCenter->SetupAttachment(RootComponent);
	Field->SetupAttachment(RootComponent);
	Field->OnComponentBeginOverlap.AddDynamic(this, &ACubicalField::OnOverlapBegin);
	Field->OnComponentEndOverlap.AddDynamic(this, &ACubicalField::OnComponentEndOverlap);

	Gu = GetActorUpVector();
	Gr = GetActorRightVector();
	Gf = GetActorForwardVector();
}

// Called when the game starts or when spawned
void ACubicalField::BeginPlay()
{
	Super::BeginPlay();
	HalfSize = MeshCenter->GetComponentScale().Z * 100;
}

void ACubicalField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		ChangeGravity = true;
	}
}

void ACubicalField::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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
void ACubicalField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChangeGravity)
	{
		float isUpDown = (PlayerRef->GetActorLocation() - GetActorLocation()).Dot(Gu);
		float isForwardBehind = (PlayerRef->GetActorLocation() - GetActorLocation()).Dot(Gf);
		float isRightLeft = (PlayerRef->GetActorLocation() - GetActorLocation()).Dot(Gr);

		/* Apply Up Gravity */
		if (UKismetMathLibrary::Abs(isUpDown) > HalfSize / 2 && isUpDown > 0)
		{
			PlayerRef->AddActorWorldOffset( -Gu * FieldForce * DeltaTime, true);
		}
		else if (UKismetMathLibrary::Abs(isUpDown) > HalfSize / 2 && isUpDown < 0)
		{
			PlayerRef->AddActorWorldOffset( Gu * FieldForce * DeltaTime, true);
		}

		/* Apply Forward Gravity */
		if (UKismetMathLibrary::Abs(isForwardBehind) > HalfSize / 2 && isForwardBehind > 0)
		{
			PlayerRef->AddActorWorldOffset( -Gf * FieldForce * DeltaTime, true);
		}
		else if (UKismetMathLibrary::Abs(isForwardBehind) > HalfSize / 2 && isForwardBehind < 0)
		{
			PlayerRef->AddActorWorldOffset( Gf * FieldForce * DeltaTime, true);
		}

		/* Apply Right Gravity */
		if (UKismetMathLibrary::Abs(isRightLeft) > HalfSize / 2 && isRightLeft > 0)
		{
			PlayerRef->AddActorWorldOffset( -Gr * FieldForce * 2 * DeltaTime, true);
		}
		else if (UKismetMathLibrary::Abs(isRightLeft) > HalfSize / 2 && isRightLeft < 0)
		{
			PlayerRef->AddActorWorldOffset( Gr * FieldForce * DeltaTime, true);
		}
	}
}

