// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	Stats.Add(EStatTypes::MaxHealth, 500.0f);
	Stats.Add(EStatTypes::Health, 500.0f);
	Stats.Add(EStatTypes::Experience, 0.0f);
	Stats.Add(EStatTypes::Damage, 50.0f);
	Stats.Add(EStatTypes::Level, 1.0f);
}

float UStatComponent::GetStat(EStatTypes StatType) const
{
	if (const float* FoundStat = Stats.Find(StatType))
	{
		return *FoundStat;
	}
	return 0.0f;
}

void UStatComponent::UpdateStat(EStatTypes StatType, float NewValue)
{
	Stats.Add(StatType, NewValue);

	if (OnStatChanged.IsBound())
	{
		OnStatChanged.Broadcast(StatType, NewValue);
	}
}

void UStatComponent::UpgradeStat(EStatTypes StatToUpgrade)
{
	switch (StatToUpgrade)
	{
	case EStatTypes::MaxHealth:
		{
			const float NewMaxHealth = GetStat(EStatTypes::MaxHealth) + 20.0f; // 업그레이드 수치는 원하는 만큼 조절
			UpdateStat(EStatTypes::MaxHealth, NewMaxHealth);
			UpdateStat(EStatTypes::Health, NewMaxHealth); // 체력을 모두 회복
			break;
		}
	case EStatTypes::Damage:
		{
			const float NewDamage = GetStat(EStatTypes::Damage) + 5.0f;
			UpdateStat(EStatTypes::Damage, NewDamage);
			break;
		}
	// 여기에 다른 스탯(MoveSpeed, AttackSpeed 등) 업그레이드 로직을 추가할 수 있습니다.
	/*
	case EStatTypes::MoveSpeed:
		{
			// 예시: 캐릭터 무브먼트 컴포넌트를 가져와서 속도를 직접 조절해야 할 수도 있습니다.
			break;
		}
	*/
	default:
		break;
	}
}

void UStatComponent::ApplyDamage(float DamageAmount)
{
	const float CurrentHealth = GetStat(EStatTypes::Health);
	const float MaxHealth = GetStat(EStatTypes::MaxHealth);

	const float DamagedHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	UpdateStat(EStatTypes::Health, DamagedHealth);
}

void UStatComponent::AddExperience(float AmountToAdd)
{
	float CurrentExperience = GetStat(EStatTypes::Experience);
	float CurrentLevel = GetStat(EStatTypes::Level);

	CurrentExperience += AmountToAdd;

	float RequiredExperienceForLevelUp = (CurrentLevel - 1) * 10.0f + 100.0f;

	while (CurrentExperience >= RequiredExperienceForLevelUp)
	{
		CurrentExperience -= RequiredExperienceForLevelUp;
		CurrentLevel += 1.0f;

		UpdateStat(EStatTypes::Level, CurrentLevel);

		const float NewMaxHealth = GetStat(EStatTypes::MaxHealth) + 10.0f;
		UpdateStat(EStatTypes::MaxHealth, NewMaxHealth);
		UpdateStat(EStatTypes::Health, NewMaxHealth); // 체력 완전 회복

		UE_LOG(LogTemp, Warning, TEXT("Level Up! New Level: %.0f"), CurrentLevel);

		if (OnLevelUp.IsBound())
		{
			OnLevelUp.Broadcast();
		}

		RequiredExperienceForLevelUp = (CurrentLevel - 1) * 10.0f + 100.0f;
	}

	UpdateStat(EStatTypes::Experience, CurrentExperience);
}

const TMap<EStatTypes, float>& UStatComponent::GetAllStats() const
{
	return Stats;
}
