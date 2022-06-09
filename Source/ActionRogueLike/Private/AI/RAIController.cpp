// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ARAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI Controller")))
	{
		RunBehaviorTree(BehaviorTree);
	}
}
