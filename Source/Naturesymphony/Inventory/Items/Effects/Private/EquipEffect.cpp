// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Public/EquipEffect.h"

#include "Naturesymphony/Characters/Public/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Naturesymphony/Characters/Public/MainPlayerAnimInstance.h"

AEquipEffect::AEquipEffect()
{
}

void AEquipEffect::BeginPlay()
{
	FirstEquipItem();
}

// Function first equip item on player
void AEquipEffect::FirstEquipItem()
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
				MainCharacterMesh = MainCharacter->GetMesh();
				if (MainCharacterMesh)
				{
					MainCharacter->CurrentWeapon = this;
					this->AttachToComponent(MainCharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MainCharacter->FirstAttachSocketName);
				}
			}
		}
	}
}

// Setter for IsAttachedToHand
void AEquipEffect::SetAttachedToHand(bool IsAttachedToHand)
{
	UAnimInstance* AnimInstance = MainCharacterMesh->GetAnimInstance();
	if (AnimInstance)
	{
		UMainPlayerAnimInstance* MainPlayerAnimInstance = Cast<UMainPlayerAnimInstance>(AnimInstance);
		if(MainPlayerAnimInstance)
		{
			bool IsInterface = MainPlayerAnimInstance->GetClass()->ImplementsInterface(UAnimInstanceInterface::StaticClass());
			if (IsInterface)
			{
				IsAttachedToHands = IsAttachedToHand;
				MainPlayerAnimInstance->Execute_UpdateWeaponAttachedToHand(MainPlayerAnimInstance, IsAttachedToHands);
			}
		}
	}
}