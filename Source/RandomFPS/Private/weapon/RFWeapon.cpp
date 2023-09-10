// Fill out your copyright notice in the Description page of Project Settings.


#include "weapon/RFWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "RandomFPS/RandomFPSCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ARFWeapon::ARFWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickUpTip = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUp"));
	PickUpTip->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ARFWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ARFWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ARFWeapon::OnSphereEndlap);
	}
	if (PickUpTip)
	{
		PickUpTip->SetVisibility(false);
	}
}

void ARFWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARFWeapon, WeaponState);
}

void ARFWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARandomFPSCharacter* RFCharacter = Cast<ARandomFPSCharacter>(OtherActor);
	if (RFCharacter)
	{
		RFCharacter->SetOverlappedWeapon(this);
	}
}

void ARFWeapon::OnSphereEndlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ARandomFPSCharacter* RFCharacter = Cast<ARandomFPSCharacter>(OtherActor);
	if (RFCharacter)
	{
		RFCharacter->SetOverlappedWeapon(nullptr);
	}
}

// Called every frame
void ARFWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARFWeapon::ShowPickUpTipWidget(bool bVisible) const
{
	if (PickUpTip)
	{
		PickUpTip->SetVisibility(bVisible);
	}
}

void ARFWeapon::SetWeaponState(EWeaponState state)
{
	WeaponState = state;
	WeaponStateUpdate();
}

void ARFWeapon::OnRep_WeaponStateChange()
{
	WeaponStateUpdate();
}

void ARFWeapon::WeaponStateUpdate()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickUpTipWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EWeaponState::EWS_Dropped:
		break;
	case EWeaponState::EWS_MAX:
		break;
	default:
		break;
	}
}

