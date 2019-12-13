// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("SensingComponent"));

  GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

  PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
  PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);

  OrginalRotation = GetActorRotation();

  if (bPatrol)
  {
    MoveToNextControlPoint();
  }
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
  if (!SeenPawn)
  {
    return;
  }

  DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

  AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
  if (GM)
  {
    GM->CompletedMission(SeenPawn, false);
  }

  SetGuardState(EAIState::Alerted);

  AController* AIController = GetController();
  if (AIController)
  {
    AIController->StopMovement();
  }
}

void AFPSAIGuard::OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
  if (GuardState == EAIState::Alerted)
  {
    return;
  }

  DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

  FVector Direction = Location - GetActorLocation();
  Direction.Normalize();

  FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
  NewLookAt.Pitch = 0.0f;
  NewLookAt.Roll = 0.0f;

  SetActorRotation(NewLookAt);

  GetWorldTimerManager().ClearTimer(TimmerHandel_RestRotation);
  GetWorldTimerManager().SetTimer(TimmerHandel_RestRotation, this, &AFPSAIGuard::ResetRotation, 3.0f);

  SetGuardState(EAIState::Suspicious);

  AController* AIController = GetController();
  if (AIController)
  {
    AIController->StopMovement();
  }
}

void AFPSAIGuard::ResetRotation()
{
  if (GuardState == EAIState::Alerted)
  {
    return;
  }

  SetActorRotation(OrginalRotation);

  SetGuardState(EAIState::Idle);

  // Stop investigating and resume patrol if patrol was set
  if (bPatrol)
  {
    MoveToNextControlPoint();
  }
}

// will only happen on client
void AFPSAIGuard::OnRep_GuardState()
{
  OnStateChange(GuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
  if (GuardState == NewState)
  {
    return;
  }

  GuardState = NewState;
  OnRep_GuardState();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (CurrentPoint)
  {
    FVector Delta = GetActorLocation() - CurrentPoint->GetActorLocation();
    float DistanceToGoal = Delta.Size();

    if (bShowAcceptanceRadiusDebug)
    {
      UE_LOG(LogTemp, Log, TEXT("Distance to goal %f"), DistanceToGoal)
    }

    if (DistanceToGoal < AcceptanceRadius)
    {
      MoveToNextControlPoint();
    }
  }
}

void AFPSAIGuard::MoveToNextControlPoint()
{
  // See if points exists
  if (PatrolPoint1 && PatrolPoint2)
  {
    if (CurrentPoint == nullptr || CurrentPoint == PatrolPoint2)
    {
      CurrentPoint = PatrolPoint1;
    }
    else
    {
      CurrentPoint = PatrolPoint2;
    }
    UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPoint);
  }
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
  Super::GetLifetimeReplicatedProps(OutLifetimeProps);

  DOREPLIFETIME(AFPSAIGuard, GuardState);
}
