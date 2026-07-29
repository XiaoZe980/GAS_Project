// Copyright GAS_Project. All Rights Reserved.

#include "GAS_AbilitySystemComponent.h"

UGAS_AbilitySystemComponent::UGAS_AbilitySystemComponent()
{
}

void UGAS_AbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
}

FGameplayAbilitySpecHandle UGAS_AbilitySystemComponent::GrantAbility(
	TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, UObject* SourceObject)
{
	if (!IsValid(AbilityClass))
	{
		return FGameplayAbilitySpecHandle();
	}

	FGameplayAbilitySpec Spec(AbilityClass, Level, INDEX_NONE, SourceObject);
	return GiveAbility(Spec);
}

void UGAS_AbilitySystemComponent::GrantAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int32 Level)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilityClasses)
	{
		GrantAbility(AbilityClass, Level);
	}
}

bool UGAS_AbilitySystemComponent::HasAbility(TSubclassOf<UGameplayAbility> AbilityClass) const
{
	if (!IsValid(AbilityClass))
	{
		return false;
	}

	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (Spec.Ability && Spec.Ability->GetClass() == AbilityClass)
		{
			return true;
		}
	}

	return false;
}

FActiveGameplayEffectHandle UGAS_AbilitySystemComponent::ApplyEffectToSelf(
	TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	if (!IsValid(EffectClass))
	{
		UE_LOG(LogGAS_Project, Error, TEXT("❌ ApplyEffectToSelf: EffectClass 为空"));
		return FActiveGameplayEffectHandle();
	}

	UE_LOG(LogGAS_Project, Warning, TEXT("💊 ApplyEffectToSelf: 正在施加 %s (Level %.0f)"), *EffectClass->GetName(), Level);

	const FGameplayEffectContextHandle Context = MakeEffectContext();
	const FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(EffectClass, Level, Context);
	const FActiveGameplayEffectHandle Handle = ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

	if (Handle.IsValid())
	{
		UE_LOG(LogGAS_Project, Warning, TEXT("✅ ApplyEffectToSelf: 效果已成功施加"));
	}
	else
	{
		UE_LOG(LogGAS_Project, Error, TEXT("❌ ApplyEffectToSelf: 施加失败，EffectSpec 可能无效"));
	}

	return Handle;
}

FActiveGameplayEffectHandle UGAS_AbilitySystemComponent::ApplyEffectToTarget(
	UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> EffectClass, float Level)
{
	if (!IsValid(Target) || !IsValid(EffectClass))
	{
		return FActiveGameplayEffectHandle();
	}

	const FGameplayEffectContextHandle Context = MakeEffectContext();
	const FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(EffectClass, Level, Context);
	return Target->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

bool UGAS_AbilitySystemComponent::HasTag(const FGameplayTag& Tag) const
{
	return HasMatchingGameplayTag(Tag);
}

bool UGAS_AbilitySystemComponent::HasAnyTag(const FGameplayTagContainer& Tags) const
{
	return HasAnyMatchingGameplayTags(Tags);
}

float UGAS_AbilitySystemComponent::GetAttributeValue(
	TSubclassOf<UAttributeSet> AttributeSetClass, FGameplayAttribute Attribute) const
{
	if (!IsValid(AttributeSetClass) || !Attribute.IsValid())
	{
		return 0.f;
	}

	const UAttributeSet* AttributeSet = GetAttributeSet(AttributeSetClass);
	if (!AttributeSet)
	{
		return 0.f;
	}

	return Attribute.GetNumericValue(AttributeSet);
}
