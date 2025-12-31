// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;

UCLASS()
class GRADUATE_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void ActivateSpawner();
protected:
	virtual void BeginPlay() override;

	// 스폰할 적 클래스
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<APawn> EnemyClass;

	// 스폰 간격 (초)
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;
    
	// 최대 스폰 수
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 MaxSpawnCount;

	// 스폰 반경 (이 액터를 중심으로 한 원형 범위)
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	float SpawnRadius = 1500.0f;

private:
	// 스폰을 실행하는 함수
	void SpawnEnemy();

	// 현재까지 스폰한 수
	int32 CurrentSpawnCount;

	// 스폰 타이머 핸들
	FTimerHandle SpawnTimerHandle;
};
