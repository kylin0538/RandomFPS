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
	friend class ARandomFPSCharacter;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void EquipRFWeapon(class ARFWeapon* WeaponToEquip);
	void SetCharacter(class ARandomFPSCharacter* CharacterToSet);
	void SetIsAniming(bool isAnim);
private:
	class ARandomFPSCharacter* Character;

	UPROPERTY(Replicated)
	class ARFWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool IsAniming{false};
		
};
