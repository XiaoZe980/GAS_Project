// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_ProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GAS_Project.h"

// GAS includes
#include "GAS/GAS_AbilitySystemComponent.h"
#include "GAS/GAS_AttributeSet.h"
#include "GAS/GAS_GameplayAbility.h"
#include "GameplayTagContainer.h"

AGAS_ProjectCharacter::AGAS_ProjectCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// --- GAS ---
	AbilitySystemComponent = CreateDefaultSubobject<UGAS_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

void AGAS_ProjectCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributesAndAbilities();
}

UAbilitySystemComponent* AGAS_ProjectCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGAS_ProjectCharacter::InitializeAttributesAndAbilities()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	if (const UGAS_AttributeSet* ExistingSet = Cast<UGAS_AttributeSet>(AbilitySystemComponent->GetAttributeSet(UGAS_AttributeSet::StaticClass())))
	{
		AttributeSet = const_cast<UGAS_AttributeSet*>(ExistingSet);
	}
	else
	{
		AttributeSet = NewObject<UGAS_AttributeSet>(this, TEXT("AttributeSet"));
		AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);
	}

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			AbilitySystemComponent->GrantAbility(AbilityClass);
		}
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultEffects)
	{
		if (EffectClass)
		{
			AbilitySystemComponent->ApplyEffectToSelf(EffectClass);
		}
	}
}

// ------------------------------------------------------------------------
// Input
// ------------------------------------------------------------------------

void AGAS_ProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_ProjectCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AGAS_ProjectCharacter::Look);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_ProjectCharacter::Look);

			// 攻击输入 — 按下时尝试激活技能，松开时通知 ASC
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AGAS_ProjectCharacter::OnAttackPressed);
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &AGAS_ProjectCharacter::OnAttackReleased);
	}
	else
	{
		UE_LOG(LogGAS_Project, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AGAS_ProjectCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AGAS_ProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AGAS_ProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AGAS_ProjectCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AGAS_ProjectCharacter::DoJumpStart()
{
	Jump();
}

void AGAS_ProjectCharacter::DoJumpEnd()
{
	StopJumping();
}

// ------------------------------------------------------------------------
// 攻击输入 → GAS Tag 激活
// ------------------------------------------------------------------------

void AGAS_ProjectCharacter::OnAttackPressed()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 用 Tag 匹配技能：
	// 所有带有 Ability.Attack.Light 这个 AbilityTag 的技能都会被尝试激活
	// ASC 会自动检查 Cost/Cooldown/激活条件，不满足则跳过
	FGameplayTagContainer AttackTags;
	AttackTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack.Light")));

	AbilitySystemComponent->TryActivateAbilitiesByTag(AttackTags);
}

void AGAS_ProjectCharacter::OnAttackReleased()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	// 通知 ASC 攻击输入已松开
	// 配合"按住蓄力"类技能使用 — 松开时技能内部可以检测到并执行释放逻辑
	FGameplayTagContainer AttackTags;
	AttackTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("Ability.Attack.Light")));

	AbilitySystemComponent->CancelAbilities(&AttackTags);
}
