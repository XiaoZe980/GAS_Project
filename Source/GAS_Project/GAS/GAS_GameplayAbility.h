// Copyright GAS_Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GAS_GameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EGAS_AbilityActivationPolicy : uint8
{
	OnInputPressed,
	OnTagAdded,
	OnEvent,
	Manual,
};

UCLASS(Abstract, Blueprintable)
class GAS_PROJECT_API UGAS_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGAS_GameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Ability")
	EGAS_AbilityActivationPolicy ActivationPolicy = EGAS_AbilityActivationPolicy::OnInputPressed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Ability", meta = (EditCondition = "ActivationPolicy == EGAS_AbilityActivationPolicy::OnInputPressed"))
	FGameplayTag InputTag;

	UFUNCTION(BlueprintCallable, Category = "GAS|Ability")
	class UGAS_AbilitySystemComponent* GetGAS_ASC() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Ability")
	class ACharacter* GetAvatarAsCharacter() const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Ability")
	virtual void EndAbilityBP(bool bWasCancelled = false);
};
