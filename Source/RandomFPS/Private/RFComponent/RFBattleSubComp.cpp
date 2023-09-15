// Fill out your copyright notice in the Description page of Project Settings.


#include "RFComponent/RFBattleSubComp.h"
#include "weapon/RFWeapon.h"
#include "RandomFPS/RandomFPSCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
URFBattleSubComp::URFBattleSubComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URFBattleSubComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URFBattleSubComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URFBattleSubComp, EquippedWeapon);
	DOREPLIFETIME(URFBattleSubComp, IsAniming);
}


// Called every frame
void URFBattleSubComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URFBattleSubComp::EquipRFWeapon(ARFWeapon* WeaponToEquip)
{
	if (!Character || !WeaponToEquip)
		return;

	EquippedWeapon = WeaponToEquip;
	const USkeletalMeshSocket* socket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (socket)
	{
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		EquippedWeapon->SetOwner(Character);
		socket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
}

void URFBattleSubComp::SetCharacter(ARandomFPSCharacter* CharacterToSet)
{
	Character = CharacterToSet;
}

void URFBattleSubComp::SetIsAniming(bool isAnim)
{
	IsAniming = isAnim;
}
