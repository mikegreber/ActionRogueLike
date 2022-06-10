// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RBTTask_Heal.h"

#include "AIController.h"
#include "RAttributeComponent.h"

URBTTask_Heal::URBTTask_Heal()
{
	HealAmount = 100.0f;
}

EBTNodeResult::Type URBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (ensure(AIController))
	{
		APawn* MyPawn = AIController->GetPawn();
		URAttributeComponent* AttributeComponent = URAttributeComponent::GetAttributes(MyPawn);
		if (AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(MyPawn, HealAmount);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}


