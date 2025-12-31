// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKillCountChangedSignature, int32, CurrentKills, int32, TotalKills);
/**
 * 
 */
UCLASS()
class GRADUATE_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseGameMode();

	virtual void ReportCharacterDeath(AController* KillerController, AController* VictimController);
	
	UPROPERTY(BlueprintAssignable, Category = "Game Rules")
	FOnKillCountChangedSignature OnKillCountChanged;

	UFUNCTION(BlueprintCallable, Category = "Game Rules")
	void SetKillChallenge(int32 NewKillsToWin);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Rules")
	int32 KillsToWin = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game Rules")
	int32 CurrentPlayerKills = 0;
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void OnGameWon();

	UFUNCTION(BlueprintImplementableEvent, Category = "Game Rules")
	void OnGameLost();
};
