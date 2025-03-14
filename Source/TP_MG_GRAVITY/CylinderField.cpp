// Fill out your copyright notice in the Description page of Project Settings.


#include "CylinderField.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ACylinderField::ACylinderField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	UStaticMesh* Asset = MeshAsset.Object;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshCollider"));
	Mesh->SetStaticMesh(Asset);
	
	MeshCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	MeshCenter->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
	MeshCenter->SetStaticMesh(Asset);

	Mesh->SetupAttachment(RootComponent);
	MeshCenter->SetupAttachment(RootComponent);
	// Add event to the OnComponentBeginOverlap delegate
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACylinderField::OnOverlapBegin);
	Mesh->OnComponentEndOverlap.AddDynamic(this, &ACylinderField::OnComponentEndOverlap);

	Gu = GetActorUpVector();
}

// Called when the game starts or when spawned
void ACylinderField::BeginPlay()
{
	Super::BeginPlay();

	Height = MeshCenter->GetComponentScale().Z * 100;
	Base = GetActorLocation() - (Height/2) * Gu;
	//DrawDebugPoint(GetWorld(), Base, 40.0f, FColor::Red, true);
}

void ACylinderField::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AMG_Player>())
	{
		PlayerRef = Cast<AMG_Player>(OtherActor);
		ChangeGravity = true;
	}
}

void ACylinderField::OnComponentEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
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
void ACylinderField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ChangeGravity)
	{
		float isUpDown = (PlayerRef->GetActorLocation() - GetActorLocation()).Dot(Gu);
		
		if (UKismetMathLibrary::Abs(isUpDown) > Height / 2 && isUpDown > 0)
		{
			PlayerRef->AddActorWorldOffset( -Gu * FieldForce * DeltaTime, true);
		}
		else if (UKismetMathLibrary::Abs(isUpDown) > Height / 2 && isUpDown < 0)
		{
			PlayerRef->AddActorWorldOffset( Gu * FieldForce * DeltaTime, true);
		}
		else
		{
			FVector Attraction = Base + Gu * (PlayerRef->GetActorLocation() - Base).Dot(Gu);
			PlayerRef->AddActorWorldOffset((Attraction - PlayerRef->GetActorLocation()) * DeltaTime, true);
			DrawDebugLine(GetWorld(),PlayerRef->GetActorLocation(),Attraction,FColor::Red, true);
		}
	}
}

