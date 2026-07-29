// Copyright GAS_Project. All Rights Reserved.

#include "GAS_AttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UGAS_AttributeSet::UGAS_AttributeSet()
	: Health(100.f)
	, MaxHealth(100.f)
	, Mana(100.f)
	, MaxMana(100.f)
	, Stamina(100.f)
	, MaxStamina(100.f)
	, Damage(0.f)
{
}

void UGAS_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGAS_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UGAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxStamina());
	}
}

void UGAS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// 打印每个 GE 的执行信息，方便排查配置错误
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	UE_LOG(LogGAS_Project, Warning, TEXT("📨 PostGameplayEffectExecute: 属性=%s, 值=%.1f, 来源=%s"),
		*Data.EvaluatedData.Attribute.GetName(),
		Data.EvaluatedData.Magnitude,
		*SourceTags.ToStringSimple());

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		HandleDamage(Data);
	}

	ClampHealth();
	ClampMana();
	ClampStamina();
}

void UGAS_AttributeSet::HandleDamage(const FGameplayEffectModCallbackData& Data)
{
	const float IncomingDamage = GetDamage();
	const float OldHealth = GetHealth();

	if (IncomingDamage > 0.f)
	{
		const float NewHealth = FMath::Max(OldHealth - IncomingDamage, 0.f);
		SetHealth(NewHealth);

		UE_LOG(LogGAS_Project, Warning, TEXT("💥 HandleDamage: 受到 %.0f 点伤害, Health %.0f → %.0f"), IncomingDamage, OldHealth, NewHealth);
	}
	else
	{
		UE_LOG(LogGAS_Project, Warning, TEXT("⚠️ HandleDamage: Damage=%.0f (≤0，未扣血) — 检查 GE 的 Magnitude 是否设为正数"), IncomingDamage);
	}

	SetDamage(0.f);
}

void UGAS_AttributeSet::ClampHealth()
{
	SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
}

void UGAS_AttributeSet::ClampMana()
{
	SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
}

void UGAS_AttributeSet::ClampStamina()
{
	SetStamina(FMath::Clamp(GetStamina(), 0.f, GetMaxStamina()));
}

void UGAS_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Health, OldValue);
}

void UGAS_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxHealth, OldValue);
}

void UGAS_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Mana, OldValue);
}

void UGAS_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxMana, OldValue);
}

void UGAS_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, Stamina, OldValue);
}

void UGAS_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGAS_AttributeSet, MaxStamina, OldValue);
}
