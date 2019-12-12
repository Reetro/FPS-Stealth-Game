// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
  MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshComp"));
  MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  RootComponent = MeshComp;

  SphereCom = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
  SphereCom->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  SphereCom->SetCollisionResponseToAllChannels(ECR_Ignore);
  SphereCom->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
  SphereCom->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();

  PlayEffect();
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
  Super::NotifyActorBeginOverlap(OtherActor);

  PlayEffect();

  AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
  if (MyCharacter)
  {
    MyCharacter->bIsCarryingObjective = true;

    Destroy();
  }
}

void AFPSObjectiveActor::PlayEffect()
{
  UGameplayStatics::SpawnEmitterAtLocation(this, PickFX, GetActorLocation());
}
