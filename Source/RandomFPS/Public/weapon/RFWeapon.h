// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Initial UMETA(DisplayName = "Initial State"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),
	EWS_MAX UMETA(DisplayName = "Default_Max"),
};


UCLASS()
class RANDOMFPS_API ARFWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ARFWeapon();

	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
		class USphereComponent* AreaSphere;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponStateChange,VisibleAnywhere, Category = "Weapon Property")
	EWeaponState WeaponState;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Property")
	class UWidgetComponent* PickUpTip;

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor * OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	virtual void OnSphereEndlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex
		);

public:	
	virtual void Tick(float DeltaTime) override;

	void ShowPickUpTipWidget(bool bVisible) const;

	void SetWeaponState(EWeaponState state);

private:

	UFUNCTION()
	void OnRep_WeaponStateChange();

	void WeaponStateUpdate();
};
