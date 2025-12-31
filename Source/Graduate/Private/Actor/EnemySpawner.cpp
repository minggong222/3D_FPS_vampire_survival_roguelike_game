// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/EnemySpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "NavigationSystem.h"
// [추가] 높이 계산을 위한 헤더
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

AEnemySpawner::AEnemySpawner()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // 기본값 설정
    SpawnInterval = 5.0f;
    MaxSpawnCount = 1000;
    CurrentSpawnCount = 0;
    SpawnRadius = 1000.0f; // 헤더에 초기값이 없다면 여기서 설정 필요
}

void AEnemySpawner::ActivateSpawner()
{
    // 스폰 타이머 설정
    if (EnemyClass)
    {
       GetWorld()->GetTimerManager().SetTimer(
          SpawnTimerHandle,
          this,
          &AEnemySpawner::SpawnEnemy,
          SpawnInterval,
          true,
          0.0f
       );
    }
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
    ActivateSpawner();
}

void AEnemySpawner::SpawnEnemy()
{
    if (!EnemyClass || CurrentSpawnCount >= MaxSpawnCount)
    {
       GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
       return;
    }

    // 1. 현재 월드의 내비게이션 시스템을 가져옵니다.
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!NavSystem)
    {
       UE_LOG(LogTemp, Error, TEXT("Navigation System not found!"));
       return;
    }

    // 2. 스포너의 위치를 중심으로 SpawnRadius 반경 내에서 랜덤하고, "걸어갈 수 있는" 위치를 찾습니다.
    FNavLocation RandomSpawnLocation;
    bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(GetActorLocation(), SpawnRadius, RandomSpawnLocation);

    // 3. 유효한 위치를 찾았을 경우에만 스폰합니다.
    if (bFoundLocation)
    {
       // [수정] 땅에 박히지 않도록 높이(Z) 보정값 계산
       float HeightOffset = 0.0f;
       
       // 스폰할 클래스의 기본 정보(CDO)를 가져와 캡슐 높이를 확인합니다.
       ACharacter* EnemyCDO = Cast<ACharacter>(EnemyClass->GetDefaultObject());
       if (EnemyCDO && EnemyCDO->GetCapsuleComponent())
       {
           // 캡슐의 반 높이(HalfHeight) + 약간의 여유분(5~10)
           HeightOffset = EnemyCDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.0f;
       }
       else
       {
           // 캐릭터가 아닐 경우 안전하게 50 정도 띄움
           HeightOffset = 50.0f;
       }

       // 네비게이션 좌표는 바닥(0) 기준이므로 높이를 더해줍니다.
       FVector FinalSpawnLocation = RandomSpawnLocation.Location;
       FinalSpawnLocation.Z += HeightOffset;

       // 스폰 파라미터 설정
       FActorSpawnParameters SpawnParams;
       SpawnParams.Owner = this;
       SpawnParams.Instigator = GetInstigator();
       
       // [수정] 벽에 끼었을 때 억지로 스폰하지 않도록 변경
       // (AdjustIfPossibleButAlwaysSpawn -> AdjustIfPossibleButDontSpawnIfColliding)
       SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
        
       // 계산된 위치에 적 스폰
       APawn* SpawnedEnemy = GetWorld()->SpawnActor<APawn>(EnemyClass, FinalSpawnLocation, FRotator::ZeroRotator, SpawnParams);

       // [수정] 실제로 스폰에 성공했을 때만 카운트 증가
       if (SpawnedEnemy)
       {
           CurrentSpawnCount++;
       }
       else
       {
           UE_LOG(LogTemp, Warning, TEXT("Spawn failed due to collision or invalid location. Retrying next tick."));
       }
    }
    else
    {
       // 만약 유효한 위치를 못 찾으면 로그를 남깁니다.
       UE_LOG(LogTemp, Warning, TEXT("Could not find a valid spawn location on NavMesh in radius."));
    }
}