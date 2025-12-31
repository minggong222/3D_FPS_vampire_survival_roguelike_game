// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerState.h"

#include "Components/StatComponent.h"

ABasePlayerState::ABasePlayerState()
{
	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));
}
