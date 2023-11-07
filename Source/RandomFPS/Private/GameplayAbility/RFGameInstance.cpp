// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/RFGameInstance.h"
#include "AbilitySystemGlobals.h"

void URFGameInstance::Init()
{
	Super::Init();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
