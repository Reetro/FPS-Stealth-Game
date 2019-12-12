// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
  BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
  BoxCollision->SetBoxExtent(FVector(75, 75, 50));
  RootComponent = BoxCollision;

  Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
  Mesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

  BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapActor);
}

void AFPSLaunchPad::OverlapActor(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  FRotator LaunchDirection = GetActorRotation();
  LaunchDirection.Pitch += LaunchPichAngle;
  FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

  ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
  if (OtherCharacter)
  {
    OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);

    if (LaunchActivationEffect)
    {
      UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchActivationEffect, GetActorLocation());
    }
  }
  if (OtherComp && OtherComp->IsSimulatingPhysics())
  {
    OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

    if (LaunchActivationEffect)
    {
      UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchActivationEffect, GetActorLocation());
    } 
  }
}
