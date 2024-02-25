// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

class ABaseWeaponActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Function equpped weapon
	void EquipWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<ABaseWeaponActor> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	FName MeleeWeaponAttachSocketName = "MeleeHipSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapons")
	FName WeaponAttachSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* UnEquipAnimMontage;

	UPROPERTY(BlueprintReadWrite)
	ABaseWeaponActor* CurrentWeapon = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool bIsEquiped = false;

private:
	// Function spawn weapons
	void SpawnWeapons();

	// Function play anim montage for character
	void PlayAnimMontaget(UAnimMontage* Animation);
};
