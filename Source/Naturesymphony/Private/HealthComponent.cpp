// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDagame);
	}
}

// Function returns HealthPercent variable
float UHealthComponent::GetHealthPercent() const
{
	return Health / MaxHealth;
}

// Function death for Character
bool UHealthComponent::bIsDead()
{
	return Health <= 0.0f;
}

// Function delegate take damage information
void UHealthComponent::OnTakeAnyDagame(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	UE_LOG(LogTemp, Error, TEXT("Health: %f"), Health);

	if (bIsDead())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDeath!"));
		OnDeath.Broadcast();
	}
}