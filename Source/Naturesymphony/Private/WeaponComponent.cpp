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
		ABaseWeaponActor* Weapon = GetWorld()->SpawnActor<ABaseWeaponActor>(WeaponClasse);
		if (Weapon)
		{
			Weapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MeleeWeaponAttachSocketName);
			CurrentWeapon = Weapon;
		}
	}
}

void UWeaponComponent::EquipWeapon()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		if (CurrentWeapon && !bIsEquiped)
		{
			CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), WeaponAttachSocketName);
			bIsEquiped = true;
			UE_LOG(LogTemp, Error, TEXT("Socket name: %s"), *CurrentWeapon->GetAttachParentSocketName().ToString());
		}
		else
		{
			CurrentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MeleeWeaponAttachSocketName);
			bIsEquiped = false;
			UE_LOG(LogTemp, Error, TEXT("Socket name: %s"), *CurrentWeapon->GetAttachParentSocketName().ToString());
		}
	}
}