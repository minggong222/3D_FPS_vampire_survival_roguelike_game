// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/BossCharacter.h"
#include "Enemy/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimInstance.h"

ABossCharacter::ABossCharacter()
{
    // 캐릭터 자체의 Tick은 AI 컨트롤러가 제어하므로 비활성화합니다.
    PrimaryActorTick.bCanEverTick = false; 

    // 기본값 설정
    MaxHealth = 50.0f;
    CurrentHealth = MaxHealth;
    bIsDead = false;
}

void ABossCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 체력을 최대로 설정
    CurrentHealth = MaxHealth;
}

float ABossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 이미 죽었다면 데미지를 받지 않음
    if (bIsDead)
    {
        return 0.0f;
    }

    const float ActualDamage = 10.f;
    if (ActualDamage > 0.f)
    {
        CurrentHealth -= ActualDamage;
        UE_LOG(LogTemp, Warning, TEXT("Boss took %f damage. Current Health: %f"), ActualDamage, CurrentHealth);

        if (CurrentHealth <= 0.f)
        {
            Die();
        }
    }
    
    return ActualDamage;
}

void ABossCharacter::Die()
{
    if (bIsDead) return; // 이미 죽음 처리가 실행되었다면 중복 실행 방지

    bIsDead = true;
    CurrentHealth = 0.0f;
    UE_LOG(LogTemp, Warning, TEXT("Boss is dead."));

    // AI 컨트롤러의 비헤이비어 트리 중지
    if (AAIController* AIController = Cast<AAIController>(GetController()))
    {
        if(AIController->GetBrainComponent())
        {
            AIController->GetBrainComponent()->StopLogic(TEXT("Boss Died"));
        }
    }

    // 충돌 및 이동 비활성화
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();

    // 래그돌(Ragdoll) 물리 시뮬레이션 활성화
    GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
    GetMesh()->SetSimulatePhysics(true);

    // 10초 후에 액터 소멸
    SetLifeSpan(10.0f);
}

void ABossCharacter::PerformAttack(int32 AttackIndex)
{
    if (bIsDead || (GetMesh() && GetMesh()->GetAnimInstance() && GetMesh()->GetAnimInstance()->IsAnyMontagePlaying()))
    {
        return;
    }

    UAnimMontage* MontageToPlay = nullptr;
    switch (AttackIndex)
    {
    case 1: MontageToPlay = AttackMontage_1; break;
    case 2: MontageToPlay = AttackMontage_2; break;
    case 3: MontageToPlay = AttackMontage_3; break;
    default: 
        UE_LOG(LogTemp, Warning, TEXT("Invalid AttackIndex: %d"), AttackIndex);
        return;
    }

    if (MontageToPlay)
    {
        // 공격 전 플레이어를 향해 바라보게 함
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        if (PlayerCharacter)
        {
            FVector DirectionToPlayer = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            FRotator LookAtRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
            SetActorRotation(FRotator(0.f, LookAtRotation.Yaw, 0.f)); // Yaw 값만 적용하여 Z축 회전
        }

        PlayAnimMontage(MontageToPlay);
        UE_LOG(LogTemp, Warning, TEXT("Performing Attack %d"), AttackIndex);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AttackMontage for index %d is not set!"), AttackIndex);
    }
}

float ABossCharacter::GetHealthPercentage() const
{
    return (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
}