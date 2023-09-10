// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RFBattleSubComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RANDOMFPS_API URFBattleSubComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URFBattleSubComp();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;

public:
	void EquipRFWeapon(class ARFWeapon* WeaponToEquip);
	void SetCharacter(class ARandomFPSCharacter* CharacterToSet);
private:
	class ARandomFPSCharacter* Character;
	class ARFWeapon* EquippedWeapon;
		
};
