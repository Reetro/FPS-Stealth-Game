// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameState.h"



AFPSGameMode::AFPSGameMode()
{
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
  DefaultPawnClass = PlayerPawnClassFinder.Class;

  // use our custom HUD class
  HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompletedMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
  if (InstigatorPawn)
  {
    if (SpectatingViewportClass)
    {
      TArray<AActor*> ReturnedActors;

      UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewportClass, ReturnedActors);

      // If SpectatingViewportClass exists then change camera to new spectator's camera on mission complete
      if (ReturnedActors.Num() > 0)
      {
        AActor* NewViewTarget = ReturnedActors[0];

        APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());

        if (PC)
        {
          PC->SetViewTargetWithBlend(NewViewTarget, 1.f, EViewTargetBlendFunction::VTBlend_Cubic);
        }
      }
    }
    else 
    {
      UE_LOG(LogTemp, Warning, TEXT("No SpectatingViewportClass please select one in the Gamemode BP"))
    }
  }

  AFPSGameState* GS = GetGameState<AFPSGameState>();
  if (GS)
  {
    GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
  }

  OnMissionComplete(InstigatorPawn, bMissionSuccess);
}

void AFPSGameMode::BeginPlay()
{
  Super::BeginPlay();
  GameStateClass = AFPSGameState::StaticClass();
}
