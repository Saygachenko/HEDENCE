// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Public/EquipWeaponEffect.h"

#include "GameFramework/Character.h"
#include "Naturesymphony/Characters/Public/MainCharacter.h"
#include "Kismet/GameplayStatics.h"

AEquipWeaponEffect::AEquipWeaponEffect()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("SkeletalMeshComponent"));
	RootComponent = SkeletalMeshComponent;
}

void AEquipWeaponEffect::BeginPlay()
{
	EquipWeaponToHip();
}

// Function spawn weapons
void AEquipWeaponEffect::EquipWeaponToHip()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
		if (Character)
		{
			AMainCharacter* MainCharacter = Cast<AMainCharacter>(Character);
			if (MainCharacter)
			{
				USkeletalMeshComponent* MainCharacterMesh = MainCharacter->GetMesh();
				if (MainCharacterMesh)
				{
					MainCharacter->CurrentWeapon = this;
					this->AttachToComponent(MainCharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MainCharacter->ToHipWeaponAttachSocketName);
				}
			}
		}
	}
}