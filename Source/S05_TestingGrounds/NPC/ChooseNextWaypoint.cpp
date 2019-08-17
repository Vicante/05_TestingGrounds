// Fill out your copyright notice in the Description page of Project Settings.


#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRoute.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	//Get Patrol Route
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	//Warn about empty patrol points
	if (PatrolPoints.Num() == 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("A Guard is missing its patrol points."));
		return EBTNodeResult::Failed;
	}

	//Set Next Waypoint
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto IndexKey = BlackboardComponent->GetValueAsInt(Index.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(Waypoint.SelectedKeyName, PatrolPoints[IndexKey]);

	//Cycle Index
	auto NextIndex = (IndexKey + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(Index.SelectedKeyName, NextIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Next Waypoint index: %i"), IndexKey);
	return EBTNodeResult::Succeeded;
}