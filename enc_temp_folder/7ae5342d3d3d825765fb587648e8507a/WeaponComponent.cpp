// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "BaseWeaponActor.h"
#include "GameFrameWork/Character.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapons();
}

// Function spawn weapons
void UWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		ABaseWeaponActor* Weapon = GetWorld()->SpawnActor<ABaseWeaponActor>(WeaponClass);
		if (Weapon)
		{
			CurrentWeapon = Weapon;
			CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MeleeWeaponAttachSocketName);
		}
	}
}

// Function equpped weapon
void UWeaponComponent::EquippingWeapon()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character && CurrentWeapon && bIsAnimFinished)
	{
		bIsAnimFinished = false;
		if (!bIsEquiped)
		{
			Character->PlayAnimMontage(EquipAnimMontage);
			bIsEquiped = true;
		}
		else
		{
			Character->PlayAnimMontage(UnEquipAnimMontage);
			bIsEquiped = false;
		}
	}
}