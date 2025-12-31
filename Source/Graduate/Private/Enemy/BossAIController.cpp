// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/BossCharacter.h"
#include "Runtime/AIModule/Classes/AIController.h"

ABossAIController::ABossAIController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ABossCharacter* EnemyPawn = Cast<ABossCharacter>(InPawn);
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