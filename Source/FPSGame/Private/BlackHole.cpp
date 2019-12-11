// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  RootComponent = MeshComp;

  OuterSphereCom = CreateDefaultSubobject<USphereComponent>(FName("OuterCollision"));
  OuterSphereCom->SetSphereRadius(10000.0);
  OuterSphereCom->SetupAttachment(MeshComp);

  InnerSphereCom = CreateDefaultSubobject<USphereComponent>(FName("InnerCollision"));
  InnerSphereCom->SetSphereRadius(175.0);
  InnerSphereCom->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();

  InnerSphereCom->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  // Get all overlapped Components that in the outer sphere
  TArray<UPrimitiveComponent*> OverlappingComp;
  OuterSphereCom->GetOverlappingComponents(OverlappingComp);

  for (int32 i = 0; i < OverlappingComp.Num(); i++)
  {
    UPrimitiveComponent* PrimComp = OverlappingComp[i];
    if (PrimComp && PrimComp->IsSimulatingPhysics())
    {
      const float SphereRadius = OuterSphereCom->GetScaledSphereRadius();
      const float ForceStrength = -2000;

      PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
    }
  }
}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor)
  {
    OtherActor->Destroy();
  }
}
