// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Stats/Public/HealthComponent.h"

#include "GameFramework/Character.h"

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

	ComponentOwner = GetOwner();
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
	if (ComponentOwner)
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(ComponentOwner);
		if (CharacterOwner)
		{
			Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
			UE_LOG(LogTemp, Error, TEXT("Health: %f"), Health);

			if (InpactResponce)
			{
				CharacterOwner->PlayAnimMontage(InpactResponce);
			}

			if (bIsDead())
			{
				UE_LOG(LogTemp, Warning, TEXT("OnDeath!"));
				OnDeath.Broadcast();
			}
		}
	}
}