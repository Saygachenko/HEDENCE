// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


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
	void EquipWeapon();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TSubclassOf<class ABaseWeaponActor> WeaponClasse;

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	FName MeleeWeaponAttachSocketName = "MeleeHipSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	FName WeaponAttachSocketName = "WeaponSocket";

private:
	bool bIsEquiped = false;

	ABaseWeaponActor* CurrentWeapon = nullptr;

	// Function spawn weapons
	void SpawnWeapons();
};
