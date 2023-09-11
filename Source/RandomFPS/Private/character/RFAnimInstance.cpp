// Fill out your copyright notice in the Description page of Project Settings.


#include "character/RFAnimInstance.h"
#include "RandomFPS/RandomFPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void URFAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	RFCharacter = Cast<ARandomFPSCharacter>(TryGetPawnOwner());
}

void URFAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (RFCharacter == nullptr)
	{
		RFCharacter = Cast<ARandomFPSCharacter>(TryGetPawnOwner());
	}
	if (RFCharacter == nullptr) return;

	FVector Velocity = RFCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	UCharacterMovementComponent* characterMovement = RFCharacter->GetCharacterMovement();
	bIsInAir = characterMovement->IsFalling();
	bIsAccelerating = characterMovement->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bIsEquipWeapon = RFCharacter->IsWeaponEquiped();
	bIsCrouch = RFCharacter->bIsCrouched;
}
