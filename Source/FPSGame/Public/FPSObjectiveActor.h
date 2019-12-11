// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  UStaticMeshComponent* MeshComp = nullptr;

  UPROPERTY(VisibleAnywhere, Category = "Components")
  USphereComponent* SphereCom = nullptr;

  UPROPERTY(EditDefaultsOnly, Category = "FX")
  UParticleSystem* PickFX;

  void PlayEffect();

public:	
  virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
