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

// Function play anim montage for character
void UWeaponComponent::PlayAnimMontaget(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		Character->PlayAnimMontage(Animation);
	}
}

// Function equpped weapon
void UWeaponComponent::EquipWeapon()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		if (CurrentWeapon && !bIsEquiped)
		{
			PlayAnimMontaget(EquipAnimMontage);
			bIsEquiped = true;
		}
		else
		{
			PlayAnimMontaget(UnEquipAnimMontage);
			bIsEquiped = false;
		}
	}
}