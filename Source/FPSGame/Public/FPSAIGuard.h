// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;
class APawn;

UENUM(BlueprintType)
enum class EAIState : uint8 
{
  Idle,
  Suspicious,
  Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  // Should the guard patrol between it's 2 points?
  UPROPERTY(EditInstanceOnly, Category = "Patrol Settings")
  bool bPatrol = false;

  UPROPERTY(EditInstanceOnly, Category = "Patrol Settings", meta = (EditCondition = "bPatrol"))
  AActor* PatrolPoint1 = nullptr;

  UPROPERTY(EditInstanceOnly, Category = "Patrol Settings", meta = (EditCondition = "bPatrol"))
  AActor* PatrolPoint2 = nullptr;

  UPROPERTY(EditInstanceOnly, Category = "Patrol Settings", meta = (EditCondition = "bPatrol"))
  float AcceptanceRadius = 60.0f;

  UPROPERTY(EditInstanceOnly, Category = "Patrol Settings", meta = (EditCondition = "bPatrol"))
  bool bShowAcceptanceRadiusDebug = false;

  // The current point the guard is moving to
  AActor* CurrentPoint;

  void MoveToNextControlPoint();

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UPawnSensingComponent* PawnSensingComponent = nullptr;

  UFUNCTION()
  void OnPawnSeen(APawn* SeenPawn);

  UFUNCTION()
  void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

  FRotator OrginalRotation;

  UFUNCTION()
  void ResetRotation();

  FTimerHandle TimmerHandel_RestRotation;

  UPROPERTY(ReplicatedUsing=OnRep_GuardState)
  EAIState GuardState;

  UFUNCTION()
  void OnRep_GuardState();

  void SetGuardState(EAIState NewState);

  UFUNCTION(BlueprintImplementableEvent, Category = "AI")
  void OnStateChange(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

  // Sets default values for this character's properties
  AFPSAIGuard();
};
