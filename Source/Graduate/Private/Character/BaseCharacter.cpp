// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacter.h"

#include "Actor/Projectile/BaseProjectile.h"
#include "Components/StatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Enemy/BaseAIController.h"
#include "Interfaces/CombatInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Game/BaseGameMode.h"
#include "Player/BasePlayerState.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	StatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

	MuzzleSocketName = "Muzzle";
	WeaponRange = 10000.f;
	TimeBetweenShots = 0.2f;
	LastFireTime = 0.f;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::OnDestroyTimerFinished()
{
	Destroy();
}

void ABaseCharacter::StartSprinting()
{
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = 1200.f;
}

void ABaseCharacter::StopSprinting()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void ABaseCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector FowardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(FowardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABaseCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D LookVector = Instance.GetValue().Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ABaseCharacter::Attack_Implementation()
{
	if (!ProjectileClass || !Controller)
	{
		return;
	}

	if (GetWorld()->GetTimeSeconds() < LastFireTime + TimeBetweenShots)
	{
		return;
	}
	LastFireTime = GetWorld()->GetTimeSeconds();

	USceneComponent* MuzzleSourceComponent = GetMesh();
	if (!MuzzleSourceComponent) return;

	const FVector SpawnLocation = MuzzleSourceComponent->GetSocketLocation(MuzzleSocketName);

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetActorEyesViewPoint(ViewLocation, ViewRotation);

	const FVector TraceStart = ViewLocation;
	const FVector TraceEnd = TraceStart + (ViewRotation.Vector() * WeaponRange);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FVector TargetLocation = TraceEnd;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionParams))
	{
		TargetLocation = HitResult.ImpactPoint;
	}

	const FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, TargetLocation);

	SpawnRotation.Yaw += 1.5f;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void ABaseCharacter::TakeDamage_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead()) return;

	UStatComponent* MyStatComponent = GetStatComponent();
	if (!MyStatComponent) return;
	if (EventInstigator)
	{
		SetInstigator(EventInstigator->GetPawn());
	}

	MyStatComponent->ApplyDamage(DamageAmount);

	if (IsDead())
	{
		Die_Implementation();
	}
}

void ABaseCharacter::Die_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("Die_Implementation has been called for: %s"), *GetName());
	AController* VictimController = GetController();
	AActor* AttackerActor = GetInstigator();
	AController* KillerController = AttackerActor ? AttackerActor->GetInstigatorController() : nullptr;

	if (ABaseGameMode* GM = GetWorld()->GetAuthGameMode<ABaseGameMode>())
	{
		GM->ReportCharacterDeath(KillerController, VictimController);
	}
    // 1. Get the actual instigator of the damage. This is a more robust approach.
    AActor* Attacker = GetInstigator(); 
    if (Attacker)
    {
        ABaseCharacter* AttackingCharacter = Cast<ABaseCharacter>(Attacker);
        if (AttackingCharacter)
        {
            if (AttackingCharacter != this)
            {
                UStatComponent* AttackerStatComponent = AttackingCharacter->GetStatComponent();
                if (AttackerStatComponent)
                {
                    AttackerStatComponent->AddExperience(100.0f);
                }
            }
        }
    }

    // --- AI Controller and Character State Change Logic ---
    AController* CurrentController = GetController();
    if (CurrentController)
    {
        ABaseAIController* AIController = Cast<ABaseAIController>(CurrentController);
        if (AIController)
        {
            UBlackboardComponent* BlackboardComponent = AIController->GetBlackboardComponent();
            if (BlackboardComponent)
            {
                BlackboardComponent->SetValueAsBool(TEXT("IsDead"), true);
            }

            UBrainComponent* BrainComponent = AIController->GetBrainComponent();
            if (BrainComponent)
            {
                BrainComponent->StopLogic("Dead");
            }
        }
    }
    
    // Change character's physical state.
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();

    // Destroy the character after a delay.
    FTimerHandle DestroyTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABaseCharacter::OnDestroyTimerFinished, 2.0f, false);
}

bool ABaseCharacter::IsDead() const
{
	const UStatComponent* MyStatComponent = GetStatComponent();
	return MyStatComponent ? MyStatComponent->GetStat(EStatTypes::Health) <= 0.0f : true;
}

UStatComponent* ABaseCharacter::GetStatComponent() const
{
	if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
	{
		return PS->StatComponent;
	}
	return StatComponent;
}

void ABaseCharacter::ToggleStatsPanel()
{
	// 캐릭터를 조종하는 플레이어 컨트롤러를 가져옵니다.
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return; // 컨트롤러가 없으면 아무것도 하지 않음
	}

	// 스탯 창이 이미 열려있는지 확인
	if (StatPanelInstance && StatPanelInstance->IsInViewport())
	{
		// 열려있다면 뷰포트에서 제거
		StatPanelInstance->RemoveFromParent();
		StatPanelInstance = nullptr;
	}
	else
	{
		// 닫혀있다면 위젯 클래스가 유효한지 확인 후 생성
		if (StatPanelClass)
		{
			StatPanelInstance = CreateWidget<UUserWidget>(PlayerController, StatPanelClass);
			if (StatPanelInstance)
			{
				// [데이터 전달] 이 캐릭터의 스탯 컴포넌트를 가져와서 위젯의 Update 함수 호출
				if (UStatComponent* StatComp = GetStatComponent())
				{
					UE_LOG(LogTemp, Warning, TEXT("스탯 컴포넌트를 찾았습니다!"));
					FName FunctionName = FName(TEXT("UpdateStats"));
					if (StatPanelInstance->GetClass()->FindFunctionByName(FunctionName))
					{
						struct FStatPanelUpdateParams { UStatComponent* InStatComponent; };
						FStatPanelUpdateParams Params;
						Params.InStatComponent = StatComp;
						StatPanelInstance->ProcessEvent(StatPanelInstance->GetClass()->FindFunctionByName(FunctionName), &Params);
					}
				}else{UE_LOG(LogTemp, Error, TEXT("***** 스탯 컴포넌트를 찾지 못했습니다! *****"));}
                
				// 뷰포트에 추가
				StatPanelInstance->AddToViewport();
			}
		}
	}
}
