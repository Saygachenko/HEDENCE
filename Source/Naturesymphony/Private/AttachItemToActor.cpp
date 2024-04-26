// Fill out your copyright notice in the Description page of Project Settings.


#include "AttachItemToActor.h"

#include "GameFramework/Character.h"
#include "Naturesymphony/Components/Public/InventorySystemComponent.h"
#include "Naturesymphony/Inventory/Items/Effects/Weapons/Public/BaseWeapon.h"

//bool UAttachItemToActor::Received_Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) const
//{
//	AActor* Owner = MeshComp->GetOwner();
//	if (Owner)
//	{
//		ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
//		if (CharacterOwner)
//		{
//			UInventorySystemComponent* InventorySystem = CharacterOwner->GetComponentByClass<UInventorySystemComponent>();
//			if (InventorySystem)
//			{
//				ABaseWeapon* CurrentWeapon = InventorySystem->MainWeapon;
//				if (CurrentWeapon && AttachToHand)
//				{
//					CurrentWeapon->AttachActor(CurrentWeapon->HandSocketName);
//
//					return true;
//				}
//				else
//				{
//					CurrentWeapon->AttachActor(CurrentWeapon->AttachSocketName);
//
//					return true;
//				}
//			}
//		}
//	}
//
//	return false;
//}
