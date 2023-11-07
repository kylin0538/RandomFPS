// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "character/RFCharacterBase.h"
#include "InputActionValue.h"
#include "RandomFPSCharacter.generated.h"

class ARFWeapon;

UCLASS(config=Game)
class ARandomFPSCharacter : public ARFCharacterBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AnimAction;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponChange)
	class ARFWeapon* RFWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	class URFBattleSubComp* RFCombat;

public:
	ARandomFPSCharacter();
	virtual void PostInitializeComponents() override;
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
		
	void EquipWeapon(const FInputActionValue& Value);

	void RFCrouch(const FInputActionValue& Value);

	void AnimStart(const FInputActionValue& Value);

	void AnimComplete(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:

	UFUNCTION(Server, Reliable)
		void ServerRequestEquipWeapon();

	UFUNCTION(Server, Reliable)
		void ServerRequestChangeIsAnim(bool isAnim);

	bool IsWeaponEquiped();

	bool IsPlayerAniming();

	void SetOverlappedWeapon(ARFWeapon* OverlapRFWeapon);
private:

	UFUNCTION()
		void OnRep_WeaponChange(ARFWeapon* lastWeapon);
};

