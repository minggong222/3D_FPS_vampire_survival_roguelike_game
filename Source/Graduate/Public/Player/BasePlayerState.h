// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class UStatComponent;
/**
 * 
 */
UCLASS()
class GRADUATE_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStatComponent> StatComponent;
};
