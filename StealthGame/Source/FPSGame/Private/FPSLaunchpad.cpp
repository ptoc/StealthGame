// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchpad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFrameWork/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchpad::AFPSLaunchpad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = OverlapComp;
	

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	// Bind overlap mesh to compoment
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchpad::OverlapLaunchPad);

	LaunchStrength = 1500;
	LaunchPitchAngle = 35.0f;


}

void AFPSLaunchpad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation(), FRotator::ZeroRotator, ScaleFX, true);
}

void AFPSLaunchpad::PlaySound()
{
	UGameplayStatics::SpawnSoundAtLocation(this, LaunchCue, GetActorLocation());
}


void AFPSLaunchpad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;


	ACharacter* OtherCharacter = Cast<ACharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);


		PlayEffects();
		PlaySound();
	}
}

