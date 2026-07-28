// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Logging/LogMacros.h"
#include "GAS_ProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UGAS_AbilitySystemComponent;
class UGAS_AttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 * Base player-controllable character for GAS_Project.
 * Implements IAbilitySystemInterface and owns its own ASC + AttributeSet.
 */
UCLASS(abstract)
class AGAS_ProjectCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// ------------------------------------------------------------------------
	// Components
	// ------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	// ------------------------------------------------------------------------
	// GAS
	// ------------------------------------------------------------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS", meta = (AllowPrivateAccess = "true"))
	UGAS_AbilitySystemComponent* AbilitySystemComponent;

	// ------------------------------------------------------------------------
	// Input
	// ------------------------------------------------------------------------

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	/** 攻击输入 — 按下时尝试激活带对应 AbilityTag 的技能 */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

public:
	AGAS_ProjectCharacter();

protected:
	virtual void BeginPlay() override;

public:
	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	// ------------------------------------------------------------------------
	// GAS Setup
	// ------------------------------------------------------------------------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Defaults")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Defaults")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

protected:
	UPROPERTY()
	UGAS_AttributeSet* AttributeSet;

	virtual void InitializeAttributesAndAbilities();

	// ------------------------------------------------------------------------
	// Input
	// ------------------------------------------------------------------------

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	/** 按下攻击键 → 通过 GAS Tag 激活技能 */
	void OnAttackPressed();
	/** 松开攻击键 → 通知 ASC 释放输入（配合蓄力/按住类技能使用） */
	void OnAttackReleased();

public:
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
