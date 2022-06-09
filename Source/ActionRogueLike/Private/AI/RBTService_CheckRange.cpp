// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/RBTService_CheckRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void URBTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI pawn and target actor
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		if (AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActorKey.SelectedKeyName)))
		{
			AAIController* MyController = OwnerComp.GetAIOwner();
			if (ensure(MyController))
			{
				APawn* AIPawn = MyController->GetPawn();
				if (ensure(AIPawn))
				{
					float DistanceToSq = FVector::DistSquared(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bWithinRange = DistanceToSq < Range * Range;
					bool bHasLineOfSight = false;
					
					if (bWithinRange)
					{
						bHasLineOfSight = MyController->LineOfSightTo(TargetActor);
					}

					BlackboardComp->SetValueAsBool(InRangeKey.SelectedKeyName, bWithinRange && bHasLineOfSight);
				}
			}
		}
	}
}
