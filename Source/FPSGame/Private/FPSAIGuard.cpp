// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

  PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
  PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);

  OrginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
  if (!SeenPawn)
  {
    return;
  }

  DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
  DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

  FVector Direction = Location - GetActorLocation();
  Direction.Normalize();

  FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
  NewLookAt.Pitch = 0.0f;
  NewLookAt.Roll = 0.0f;

  SetActorRotation(NewLookAt);

  GetWorldTimerManager().ClearTimer(TimmerHandel_RestRotation);
  GetWorldTimerManager().SetTimer(TimmerHandel_RestRotation, this, &AFPSAIGuard::ResetRotation, 3.0f);
}

void AFPSAIGuard::ResetRotation()
{
  SetActorRotation(OrginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
