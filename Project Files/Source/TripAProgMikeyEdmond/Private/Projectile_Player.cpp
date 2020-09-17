// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile_Player.h"
#include "ShootRangeTarget.h"
#include "AIChar_Abstract_Turret.h"

AProjectile_Player::AProjectile_Player()
{
	// For Overlaps
	projCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Player::OnOverlapBegin);
}

void AProjectile_Player::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (AShootRangeTarget* EnemyTarget = Cast<AShootRangeTarget>(OtherActor))
		{
			EnemyTarget->AShootRangeTarget::DoDamage(AProjectile_Abstract::DamageValue);
			Destroy(this);
		}
		if (AAIChar_Abstract_Turret* EnemyTurret = Cast<AAIChar_Abstract_Turret>(OtherActor))
		{
			EnemyTurret->AAIChar_Abstract_Turret::DoDamage(AProjectile_Abstract::DamageValue);
			Destroy(this);
		}
	}
}