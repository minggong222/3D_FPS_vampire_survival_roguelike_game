// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BaseEnemy.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/AIController.h"

ABaseAIController::ABaseAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABaseEnemy* EnemyPawn = Cast<ABaseEnemy>(InPawn);
	if (EnemyPawn)
	{
		UBehaviorTree* BTree = EnemyPawn->GetBehaviorTree();
		if (BTree && BTree->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*(BTree->BlackboardAsset));
			BehaviorTreeComponent->StartTree(*BTree);
		}
	}
}
