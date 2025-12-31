// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseGameMode.h"

#include "Player/BasePlayerState.h"

ABaseGameMode::ABaseGameMode()
{
	PlayerStateClass = ABasePlayerState::StaticClass();
}

void ABaseGameMode::ReportCharacterDeath(AController* KillerController, AController* VictimController)
{
	bool bKillsChanged = false;

	if (VictimController && VictimController->IsPlayerController())
	{
		UE_LOG(LogTemp, Error, TEXT("==== PLAYER DIED! ===="));
		if (CurrentPlayerKills != 0)
		{
			CurrentPlayerKills = 0;
			bKillsChanged = true;
		}
		OnGameLost();
	}
	else if (KillerController && KillerController->IsPlayerController())
	{
		if (KillsToWin > 0)
		{
			CurrentPlayerKills++;
			bKillsChanged = true;
			UE_LOG(LogTemp, Warning, TEXT("Nice Kill. 현재 킬: %d / %d"), CurrentPlayerKills, KillsToWin);
			if (CurrentPlayerKills >= KillsToWin)
			{
				OnGameWon();
			}
		}
	}
	
	if (bKillsChanged && OnKillCountChanged.IsBound())
	{
		OnKillCountChanged.Broadcast(CurrentPlayerKills, KillsToWin);
	}
}

void ABaseGameMode::SetKillChallenge(int32 NewKillsToWin)
{
	KillsToWin = NewKillsToWin;
	CurrentPlayerKills = 0;

	if (OnKillCountChanged.IsBound())
	{
		OnKillCountChanged.Broadcast(CurrentPlayerKills, KillsToWin);
	}
}
