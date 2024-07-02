// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Naturesymphony/Enums/Characters/Public/CharacterState.h"
#include "Naturesymphony/Enums/Combat/Public/CharacterAction.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateBeginSignature, const ECharacterState&, CharacterState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEndSignature, const ECharacterState&, CharacterState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterActionBeginSignature, const ECharacterAction&, CharacterAction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterActionEndSignature, const ECharacterAction&, CharacterAction);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnStateBeginSignature OnStateBegin;

	UPROPERTY(BlueprintAssignable)
	FOnStateEndSignature OnStateEnd;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterActionBeginSignature OnCharacterActionBegin;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterActionEndSignature OnCharacterActionEnd;

	// Sets default values for this component's properties
	UStateManagerComponent();

	UFUNCTION(BlueprintPure)
	ECharacterState GetCurrentState() { return CurrentState; };

	UFUNCTION(BlueprintCallable)
	void SetState(ECharacterState NewState);

	UFUNCTION(BlueprintCallable)
	void ResetState();

	UFUNCTION(BlueprintPure)
	bool IsCurrentStateEqualToAny(TArray<ECharacterState> StatesToCheckArray);

	UFUNCTION(BlueprintCallable)
	void SetCurrentAction(ECharacterAction NewCharacterAction);

	UFUNCTION(BlueprintPure)
	ECharacterAction GetCurrentCtion() { return CurrentCharacterAction; };

	UFUNCTION(BlueprintPure)
	bool IsCurrentActionEqualToAny(TArray<ECharacterAction> ActionsToCheckArray);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	ECharacterState CurrentState = ECharacterState::None;
	ECharacterAction CurrentCharacterAction = ECharacterAction::None;
};
