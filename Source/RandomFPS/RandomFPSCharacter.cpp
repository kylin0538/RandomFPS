// Copyright Epic Games, Inc. All Rights Reserved.

#include "RandomFPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Net/UnrealNetwork.h>
#include "RFComponent/RFBattleSubComp.h"
#include "weapon/RFWeapon.h"


//////////////////////////////////////////////////////////////////////////
// ARandomFPSCharacter

ARandomFPSCharacter::ARandomFPSCharacter()
{
	// Set size for collision capsule
	bReplicates = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	RFCombat = CreateDefaultSubobject<URFBattleSubComp>(TEXT("RFCombat"));
	RFCombat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
}

void ARandomFPSCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void ARandomFPSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARandomFPSCharacter, RFWeapon, COND_OwnerOnly);
}

void ARandomFPSCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (RFCombat)
	{
		RFCombat->SetCharacter(this);
	}
}

bool ARandomFPSCharacter::IsWeaponEquiped()
{
	return RFCombat && RFCombat->EquippedWeapon;
}

bool ARandomFPSCharacter::IsPlayerAniming()
{
	return RFCombat && RFCombat->IsAniming;
}

void ARandomFPSCharacter::SetOverlappedWeapon(ARFWeapon* OverlapRFWeapon)
{	
	if (RFWeapon)
	{
		RFWeapon->ShowPickUpTipWidget(false);
	}
	RFWeapon = OverlapRFWeapon;
	if (IsLocallyControlled())
	{
		if (RFWeapon)
		{
			RFWeapon->ShowPickUpTipWidget(true);
		}
	}
}

void ARandomFPSCharacter::OnRep_WeaponChange(ARFWeapon* lastWeapon)
{
	if (lastWeapon)
	{
		lastWeapon->ShowPickUpTipWidget(false);
	}
	if (RFWeapon)
	{
		RFWeapon->ShowPickUpTipWidget(true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARandomFPSCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARandomFPSCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARandomFPSCharacter::Look);

		EnhancedInputComponent->BindAction(EquipWeaponAction, ETriggerEvent::Triggered, this, &ARandomFPSCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ARandomFPSCharacter::RFCrouch);

		EnhancedInputComponent->BindAction(AnimAction, ETriggerEvent::Started, this, &ARandomFPSCharacter::AnimStart);
		EnhancedInputComponent->BindAction(AnimAction, ETriggerEvent::Completed, this, &ARandomFPSCharacter::AnimComplete);
	}

}

void ARandomFPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARandomFPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARandomFPSCharacter::EquipWeapon(const FInputActionValue& Value)
{
	if (RFCombat)
	{
		if (HasAuthority())
		{
			RFCombat->EquipRFWeapon(RFWeapon);
		}
		else {
			ServerRequestEquipWeapon();
		}
		
	}
}

void ARandomFPSCharacter::RFCrouch(const FInputActionValue& Value)
{
	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void ARandomFPSCharacter::AnimStart(const FInputActionValue& Value)
{
	if (IsWeaponEquiped())
	{
		RFCombat->SetIsAniming(true);
		ServerRequestChangeIsAnim(true);
	}
}

void ARandomFPSCharacter::AnimComplete(const FInputActionValue& Value)
{
	if (IsWeaponEquiped())
	{
		RFCombat->SetIsAniming(false);
		ServerRequestChangeIsAnim(false);
	}
}

void ARandomFPSCharacter::ServerRequestEquipWeapon_Implementation()
{
	RFCombat->EquipRFWeapon(RFWeapon);
}

void ARandomFPSCharacter::ServerRequestChangeIsAnim_Implementation(bool isAnim)
{
	RFCombat->SetIsAniming(isAnim);
}