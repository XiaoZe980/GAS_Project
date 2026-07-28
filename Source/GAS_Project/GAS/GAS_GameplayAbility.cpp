// Copyright GAS_Project. All Rights Reserved.

#include "GAS_GameplayAbility.h"
#include "GAS_AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

UGAS_GameplayAbility::UGAS_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGAS_GameplayAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

UGAS_AbilitySystemComponent* UGAS_GameplayAbility::GetGAS_ASC() const
{
	return Cast<UGAS_AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

ACharacter* UGAS_GameplayAbility::GetAvatarAsCharacter() const
{
	return Cast<ACharacter>(GetAvatarActorFromActorInfo());
}

void UGAS_GameplayAbility::EndAbilityBP(bool bWasCancelled)
{
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, bWasCancelled);
}
