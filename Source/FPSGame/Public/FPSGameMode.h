// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameState.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

  UPROPERTY(EditDefaultsOnly, Category = "Spectating")
  TSubclassOf<AActor> SpectatingViewportClass;

public:

	AFPSGameMode();

  void CompletedMission(APawn* InstigatorPawn, bool bMissionSuccess);

  UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
  void OnMissionComplete(APawn* InstigatorPawn, bool bMissionSuccess);

  virtual void BeginPlay() override;
};
