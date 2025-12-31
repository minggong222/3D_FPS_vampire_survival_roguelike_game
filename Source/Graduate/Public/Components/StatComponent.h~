// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

UENUM(BlueprintType)
enum class EStatTypes : uint8
{
	Health,
	MaxHealth,
	Experience,
	Damage,
	Level,
	MoveSpeed,
	AttackSpeed,
	PierceCount,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChangedSignature, EStatTypes, StatType, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUpSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GRADUATE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	TMap<EStatTypes, float> Stats;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnStatChangedSignature OnStatChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelUpSignature OnLevelUp;
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetStat(EStatTypes StatType) const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateStat(EStatTypes StatType, float NewValue);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpgradeStat(EStatTypes StatToUpgrade);
	
	void ApplyDamage(float DamageAmount);

	void AddExperience(float AmountToAdd);

	UFUNCTION(BlueprintCallable, Category = "Stats")
	const TMap<EStatTypes, float>& GetAllStats() const;
};
