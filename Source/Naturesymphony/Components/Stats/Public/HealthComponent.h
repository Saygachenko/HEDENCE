// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnDeathSignature OnDeath;

	// Sets default values for this component's properties
	UHealthComponent();

	// Function returns HealthPercent variable
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	// Function death for Character
	UFUNCTION(BlueprintPure)
	bool bIsDead();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta=(ClampMin = 0.0f, ClampMax = 1000.0f))
	float MaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	TObjectPtr<UAnimMontage> InpactResponce;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	UPROPERTY()
	TObjectPtr<AActor> ComponentOwner;

	// Function delegate take damage information
	UFUNCTION()
	void OnTakeAnyDagame(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
