// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BossCharacter.generated.h"

UCLASS()
class GRADUATE_API ABossCharacter : public ABaseCharacter
{
    GENERATED_BODY()

public:
    ABossCharacter();

    /** 비헤이비어 트리를 반환하는 함수 */
    UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

    /** * 비헤이비어 트리에서 호출할 공격 수행 함수입니다.
     * @param AttackIndex 1, 2, 3 중 하나의 값을 받아 해당 공격 몽타주를 재생합니다.
     */
    UFUNCTION(BlueprintCallable, Category = "Boss Skills")
    void PerformAttack(int32 AttackIndex);

    /** 현재 체력 비율을 반환하는 함수 */
    UFUNCTION(BlueprintPure, Category = "Boss Stats")
    float GetHealthPercentage() const;

protected:
    virtual void BeginPlay() override;

    /** 데미지를 받을 때마다 호출되는 함수 */
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

    // --- AI ---
    UPROPERTY(EditAnywhere, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    // --- 보스 능력치 ---
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boss Stats")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Stats")
    float CurrentHealth;
    
    // --- 공격 몽타주 ---
    UPROPERTY(EditDefaultsOnly, Category = "Boss Skills|Animations")
    UAnimMontage* AttackMontage_1;
    
    UPROPERTY(EditDefaultsOnly, Category = "Boss Skills|Animations")
    UAnimMontage* AttackMontage_2;

    UPROPERTY(EditDefaultsOnly, Category = "Boss Skills|Animations")
    UAnimMontage* AttackMontage_3;

private:
    /** 보스가 죽었을 때 처리하는 함수 */
    void Die();

    /** 보스가 죽었는지 확인하는 플래그 */
    bool bIsDead;
};