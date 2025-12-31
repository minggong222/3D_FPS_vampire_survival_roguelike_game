// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

class UBehaviorTree;

UCLASS()
class GRADUATE_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

public:
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
