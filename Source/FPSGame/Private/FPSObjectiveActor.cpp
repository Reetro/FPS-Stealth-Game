// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	
}

// Called every frame
void AFPSObjectiveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
  Super::NotifyActorBeginOverlap(OtherActor);

  PlayEffect();
}

void AFPSObjectiveActor::PlayEffect()
{
  UGameplayStatics::SpawnEmitterAtLocation(this, PickFX, GetActorLocation());
}
