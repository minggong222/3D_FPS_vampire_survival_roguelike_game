// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/BaseCharacter.h"
#include "Components/StatComponent.h"

ABasePlayerController::ABasePlayerController()
{
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (Subsystem && DefaultMappingContext)
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	ControlledCharacter = Cast<ABaseCharacter>(GetPawn());
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Attack);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ABasePlayerController::StopJumping);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this,
		                                   &ABasePlayerController::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this,
		                                   &ABasePlayerController::StopSprinting);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
		EnhancedInputComponent->BindAction(ToggleStatsAction, ETriggerEvent::Started, this,
		                                   &ABasePlayerController::ToggleStatsPanel);
	}
}

void ABasePlayerController::Move(const FInputActionInstance& Instance)
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Move(Instance);
	}
}

void ABasePlayerController::Look(const FInputActionInstance& Instance)
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Look(Instance);
	}
}

void ABasePlayerController::StartSprinting()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StartSprinting();
	}
}

void ABasePlayerController::StopSprinting()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StopSprinting();
	}
}

void ABasePlayerController::Attack()
{
	if (ControlledCharacter)
	{
		ICombatInterface::Execute_Attack(ControlledCharacter);
	}
}

void ABasePlayerController::Jump()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void ABasePlayerController::StopJumping()
{
	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

void ABasePlayerController::ToggleStatsPanel()
{
	// 컨트롤러가 조종하는 캐릭터(Pawn)가 있는지 확인
	if (ControlledCharacter)
	{
		// 캐릭터의 토글 함수를 호출
		ControlledCharacter->ToggleStatsPanel();
	}
}