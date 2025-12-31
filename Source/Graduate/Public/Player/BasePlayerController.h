// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"	

class UInputMappingContext;
class UInputAction;
class ABaseCharacter;
class UUserWidget;
struct FInputActionInstance;

/**
 * */
UCLASS()
class GRADUATE_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

	// BaseCharacter.h에서 가져온 입력 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ToggleStatsAction;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StatPanelClass;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY()
	class ABaseCharacter* ControlledCharacter;
    
	// Input
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void StartSprinting();
	void StopSprinting();
	void Attack();
	void Jump();
	void StopJumping();
	void ToggleStatsPanel();

	UPROPERTY()
	TObjectPtr<UUserWidget> StatPanelInstance;
};
