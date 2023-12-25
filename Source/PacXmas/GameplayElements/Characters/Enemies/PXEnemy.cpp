// Copyright (c) 2023 Santa Claus. All rights reserved.


#include "PXEnemy.h"
#include "BehaviorComponent/PXEnemyBehaviorComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PacXmas/GameplayElements/Characters/AppearanceComponent/Enemies/PXEnemyAppearanceComponent.h"
#include "PacXmas/GameplayElements/Characters/Player/PXPlayer.h"
#include "PacXmas/Utilities/CustomLogs/PXCustomLogs.h"

APXEnemy::APXEnemy()
{
	CollisionComponent->SetCollisionProfileName(TEXT("Enemy"));

	BehaviorComponent = CreateDefaultSubobject<UPXEnemyBehaviorComponent>(TEXT("BehaviorComponent"));

	if (!FloatingPawnMovement)
	{
		UE_LOG(LogCharacter, Warning, TEXT("APXEnemy::APXEnemy"))
		return;
	}

	FloatingPawnMovement->MaxSpeed = 200.f;
	FloatingPawnMovement->Acceleration = 20000.f;
	FloatingPawnMovement->Deceleration = 40000.f;
	FloatingPawnMovement->TurningBoost = 40.f;

	if (!CollisionComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXEnemy::APXEnemy|CollisioComponent is nullptr"))
		return;
	}

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APXEnemy::OnOverlapBegin);

	EnemyAppearanceComponent = CreateDefaultSubobject<UPXEnemyAppearanceComponent>(TEXT("Appearance Component"));
}

void APXEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void APXEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BehaviorComponent && !bIsStunned)
	{
		const FVector Direction = BehaviorComponent->DetermineNextDirection();

		if (Direction.Equals(FVector::UpVector) || Direction.Equals(FVector::DownVector))
		{
			MoveVertical(Direction.Z);
		}
		else if (Direction.Equals(FVector::ForwardVector) || Direction.Equals(FVector::BackwardVector))
		{
			MoveHorizontal(Direction.X);
		}

		EnemyAppearanceComponent->SetFlipbookBasedOnActorForwardVector(GetActorForwardVector());
	}
}

FVector APXEnemy::GetScaledBoxExtent() const
{
	if (!CollisionComponent)
	{
		UE_LOG(LogCharacter, Warning, TEXT("APXEnemy::GetScaledBoxExtent|CollisionComponent is nullptr"))
		return FVector::ZeroVector;
	}

	return CollisionComponent->GetScaledBoxExtent();
}

void APXEnemy::EatPudding(const FHitResult& SweepResult)
{
	StunYourself(EatingPuddingTime);

	const FVector ImpactNormal = SweepResult.ImpactNormal;

	if (ImpactNormal.X > 0)
	{
		EnemyAppearanceComponent->SetFlipbookGetHitWithPudding(EEnemyGetHitPudding::Right);
	}
	else if (ImpactNormal.X < 0)
	{
		EnemyAppearanceComponent->SetFlipbookGetHitWithPudding(EEnemyGetHitPudding::Left);
	}
	else if (ImpactNormal.Z > 0)
	{
		EnemyAppearanceComponent->SetFlipbookGetHitWithPudding(EEnemyGetHitPudding::Up);
	}
	else if (ImpactNormal.Z < 0)
	{
		EnemyAppearanceComponent->SetFlipbookGetHitWithPudding(EEnemyGetHitPudding::Down);
	}
}

void APXEnemy::GetFlashed()
{
	StunYourself(FlashedTime);

	EnemyAppearanceComponent->SetFlipbookFlashed();
}

void APXEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	APXPlayer* PlayerCharacter = Cast<APXPlayer>(OtherActor);
	if (PlayerCharacter && !bIsStunned)
	{
		PlayerCharacter->LooseLife();
	}
}

void APXEnemy::StunYourself(const float Time)
{
	bIsStunned = true;

	// Check if the timer is already active and reset/extend it
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APXEnemy::ResetStun, Time);
}

void APXEnemy::ResetStun()
{
	bIsStunned = false;
}
