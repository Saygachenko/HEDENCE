// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Components/Public/CombatComponent.h"

#include "Naturesymphony/Inventory/Items/Effects/Weapons/Public/BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Naturesymphony/Characters/Public/MainPlayerAnimInstance.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Function for set main weapon
void UCombatComponent::SetMainWeapon(ABaseWeapon* NewWeapon)
{

	if (NewWeapon)
	{
		if (MainWeapon)
		{
			MainWeapon->OnUnequipped();
		}

		MainWeapon = NewWeapon;
	}

}

void UCombatComponent::SetCombatEnabled(bool CombatEnabled)
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
		if (OwnerCharacter)
		{
			USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
			if (MeshComponent)
			{
				UAnimInstance* AnimInstance = MeshComponent->GetAnimInstance();
				if (AnimInstance)
				{
					UMainPlayerAnimInstance* MainPlayerAnimInstance = Cast<UMainPlayerAnimInstance>(AnimInstance);
					if (MainPlayerAnimInstance)
					{
						bCombatEnabled = CombatEnabled;
						MainPlayerAnimInstance->Execute_UpdateCombatEnabled(MainPlayerAnimInstance, CombatEnabled);
					}
				}
			}
		}
	}
}