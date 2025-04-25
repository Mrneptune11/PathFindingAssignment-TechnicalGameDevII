// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 *
 */
UCLASS()
class ASTARALGOASSIGNMENT_API AMyAIController : public AAIController
{
	GENERATED_BODY()


public:
	AMyAIController();//constructor

	virtual void BeginPlay() override; //when loaded into world 
	virtual void Tick(float DeltaSeconds) override; //internal clock for the AI 

protected:
	UPROPERTY()
	APawn* Player;  //the player, which is the target for this path finding

	void FindPath(); //finds path to the player 

	// timer and interval system to update pathfinding
	float PathUpdateInterval;
	float TimeSinceLastPathUpdate;
};

