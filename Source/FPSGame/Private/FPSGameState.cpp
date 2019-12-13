// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess) // Needs _Impementation
{
  UWorld* World = GetWorld();

  for (FConstPawnIterator It = World->GetPawnIterator(); It; It++)
  {
    APawn* Pawn = It->Get();
    if (Pawn && Pawn->IsLocallyControlled())
    {
      Pawn->DisableInput(nullptr); // Disable input for all controlled pawn's
    } 
  }
}
