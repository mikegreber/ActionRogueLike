// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RBTService_CheckLowHealth.h"

#include "AIController.h"
#include "RAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void URBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		AAIController* MyController = OwnerComp.GetAIOwner();
		if (ensure(MyController))
		{
			APawn* AIPawn = MyController->GetPawn();
			if (ensure(AIPawn))
			{
				URAttributeComponent* AttributeComp = URAttributeComponent::GetAttributes(AIPawn);
				if (AttributeComp)
				{
					BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttributeComp->GetHealth() < HealthMin);
				}
			}
		}
	}

	
}
