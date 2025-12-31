// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "BaseCharacter.generated.h"

class ABaseProjectile;
class ABaseWeapon;
class UStatComponent;

UCLASS()
class GRADUATE_API ABaseCharacter : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	
	// Input
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void StartSprinting();
	void StopSprinting();
	void ToggleStatsPanel();


	virtual void Attack_Implementation() override;
	virtual void TakeDamage_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	UStatComponent* GetStatComponent() const;
protected:
	virtual void BeginPlay() override;

	void OnDestroyTimerFinished();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	float WeaponRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon")
	float TimeBetweenShots;

	bool bIsSprinting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStatComponent* StatComponent;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> StatPanelClass;
private:
	float LastFireTime;

	UPROPERTY()
	TObjectPtr<UUserWidget> StatPanelInstance;
};
