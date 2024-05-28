// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterState.h"
#include "StateManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateBeginSignature, const ECharacterState&, CharacterState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateEndSignature, const ECharacterState&, CharacterState);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NATURESYMPHONY_API UStateManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(BlueprintAssignable)
	FOnStateBeginSignature OnStateBegin;

	UPROPERTY(BlueprintAssignable)
	FOnStateEndSignature OnStateEnd;

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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	ECharacterState CurrentState = ECharacterState::None;
};
