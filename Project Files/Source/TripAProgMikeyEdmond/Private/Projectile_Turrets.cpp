// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile_Turrets.h"
#include "PlayerCharacter.h"

AProjectile_Turrets::AProjectile_Turrets()
{
	// For Overlaps
	projCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Turrets::OnOverlapBegin);
}

void AProjectile_Turrets::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(OtherActor))
		{
			PlayerChar->APlayerCharacter::Health_Adjust(AProjectile_Abstract::DamageValue);
			Destroy(this);
		}
	}
}