// Copyright GAS_Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GAS_AbilitySystemComponent.generated.h"

UCLASS(ClassGroup = "GAS", meta = (BlueprintSpawnableComponent))
class GAS_PROJECT_API UGAS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UGAS_AbilitySystemComponent();

	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
	FGameplayAbilitySpecHandle GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1, UObject* SourceObject = nullptr);

	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
	void GrantAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilityClasses, int32 Level = 1);

	UFUNCTION(BlueprintCallable, Category = "GAS|Abilities")
	bool HasAbility(TSubclassOf<UGameplayAbility> AbilityClass) const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Effects")
	FActiveGameplayEffectHandle ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.f);

	UFUNCTION(BlueprintCallable, Category = "GAS|Effects")
	FActiveGameplayEffectHandle ApplyEffectToTarget(UAbilitySystemComponent* Target, TSubclassOf<UGameplayEffect> EffectClass, float Level = 1.f);

	UFUNCTION(BlueprintCallable, Category = "GAS|Tags")
	bool HasTag(const FGameplayTag& Tag) const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Tags")
	bool HasAnyTag(const FGameplayTagContainer& Tags) const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Attributes")
	float GetAttributeValue(TSubclassOf<UAttributeSet> AttributeSetClass, FGameplayAttribute Attribute) const;
};
