// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ECharacterAction : uint8
{
	None,
	GeneralAction,
	LightAttack,
	StrongAttack,
	ChargedAttack,
	FallingAttack,
	Dodge,
	EnterCombat,
	ExitCombat
};