// Fill out your copyright notice in the Description page of Project Settings.


#include "Naturesymphony/Inventory/Items/Effects/Public/EquipWeaponEffect.h"

//#include "GameFramework/Character.h"
#include "Naturesymphony/Characters/Public/MainCharacter.h"
#include "Kismet/GameplayStatics.h"
//#include "Naturesymphony/Inventory/Public/AnimInstanceInterface.h"
#include "MainPlayerAnimInstance.h"

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
					UAnimInstance* AnimInstance = MainCharacterMesh->GetAnimInstance();
					if (AnimInstance)
					{
						MainPlayerAnimInstance = Cast<UMainPlayerAnimInstance>(AnimInstance);
						if (MainPlayerAnimInstance)
						{
							if (DataTable)
							{
								const FItemData* ItemData = DataTable->FindRow<FItemData>(FName("item_0001"), "");
								if (ItemData)
								{
									MainCharacter->CurrentWeapon = this;
									this->AttachToComponent(MainCharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), MainCharacter->FirstAttachSocketName);
									MainPlayerAnimInstance->Execute_UpdateCombatType(MainPlayerAnimInstance, ItemData->CombatType);
								}
							}
						}
					}
				}
			}
		}
	}
}

void AEquipWeaponEffect::SetAttachedToHand(bool IsAttachedToHand)
{
	if (MainPlayerAnimInstance)
	{
		IsAttachedToHands = IsAttachedToHand;
		MainPlayerAnimInstance->Execute_UpdateWeaponAttachedToHand(MainPlayerAnimInstance, IsAttachedToHands);
	}
}