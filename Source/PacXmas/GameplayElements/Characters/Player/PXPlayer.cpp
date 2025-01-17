// Copyright (c) 2023 Smoking Carrots. All rights reserved.


#include "PXPlayer.h"
#include "AppearanceComponent/PXPlayerAppearanceComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MovementComponent/PXPlayerMovementComponent.h"
#include "PacXmas/GameplayElements/Projectiles/Pudding/PXProjectilePudding.h"
#include "PacXmas/Utilities/CustomLogs/PXCustomLogs.h"

APXPlayer::APXPlayer()
{
	if (!CollisionComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::APXPlayer|CollisionComponent is nullptr"))
		return;
	}

	CollisionComponent->SetCollisionProfileName(TEXT("Player"));

	PXPlayerAppearanceComponent = CreateDefaultSubobject<UPXPlayerAppearanceComponent>(TEXT("Appearance Component"));
	PXPlayerMovementComponent = CreateDefaultSubobject<UPXPlayerMovementComponent>(TEXT("Movement Component"));

	PXPlayerAppearanceComponent->InitializeMovementComponent(PXPlayerMovementComponent);

	BindOnShootPuddingAnimationEndDelegate();
}

void APXPlayer::Move(const FVector& Value) const
{
	if (!PXPlayerMovementComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::Move|PXPlayerMovementComponent is nullptr"))
		return;
	}

	if (!Value.Equals(FVector::ZeroVector))
	{
		PXPlayerMovementComponent->SetNextDesiredDirection(Value);
	}
}

void APXPlayer::UpdateDataAsset() const
{
	if (!PXPlayerAppearanceComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::ChangeLook|PXPlayerAppearanceComponent is nullptr"))
		return;
	}

	if (bHasPudding && bHasMusicSheet)
	{
		PXPlayerAppearanceComponent->SetCurrentDataAsset(EPlayerLook::PuddingMusicSheet);
	}
	else if (bHasPudding)
	{
		PXPlayerAppearanceComponent->SetCurrentDataAsset(EPlayerLook::Pudding);
	}
	else if (bHasMusicSheet)
	{
		PXPlayerAppearanceComponent->SetCurrentDataAsset(EPlayerLook::MusicSheet);
	}
	else
	{
		PXPlayerAppearanceComponent->SetCurrentDataAsset(EPlayerLook::Default);
	}
}

void APXPlayer::CollectMusicSheet()
{
	bHasMusicSheet = true;
	UpdateDataAsset();
	UpdateFlipbook();
}

void APXPlayer::DropMusicSheet()
{
	bHasMusicSheet = false;
	UpdateDataAsset();
	UpdateFlipbook();
}

bool APXPlayer::GetHasMusicSheet() const
{
	return bHasMusicSheet;
}

void APXPlayer::CollectPudding()
{
	bHasPudding = true;
	UpdateDataAsset();
	UpdateFlipbook();
}

void APXPlayer::ShootPudding()
{
	if (bHasPudding)
	{
		bHasPudding = false;
		UpdateDataAsset();
		UpdateFlipbook();
		PlayThrowPuddingSound();

		PXPlayerMovementComponent->SetCanMove(false);

		OnShootPudding.Broadcast(bHasMusicSheet, GetActorForwardVector());
	}
}

bool APXPlayer::GetHasPudding() const
{
	return bHasPudding;
}

uint8_t APXPlayer::GetLives() const
{
	return Lives;
}

void APXPlayer::LooseLife()
{
	if (!bIsUntouchable)
	{
		if (Lives > 0)
		{
			Lives--;
			if (Lives > 0)
			{
				// Become untouchable for period of time
				BecomeUntouchable();
				StartBlink();
			}
			else
			{
				// Become untouchable to the end
				bIsUntouchable = true;
				HandleGameOver();
			}
		}

		PlayLooseLifeSound();
		UpdateHearts();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APXPlayer::ResumeMovement()
{
	PXPlayerMovementComponent->SetCanMove(true);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void APXPlayer::SpawnProjectilePudding()
{
	const FVector ForwardVector = GetActorForwardVector();
	const FVector SpawnLocation = GetActorLocation();
	const FRotator ProjectileRotation = ForwardVector.Rotation();

	GetWorld()->SpawnActor<APXProjectilePudding>(ProjectileClass, SpawnLocation, ProjectileRotation);
}

void APXPlayer::StartBlink() const
{
	if (!PXPlayerAppearanceComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::StartBlink|PXPlayerAppearanceComponent is nullptr"));
		return;
	}

	PXPlayerAppearanceComponent->StartBlink(UntouchableDuration);
}

void APXPlayer::PlayThrowPuddingSound() const
{
	if (!ThrowPuddingSound)
	{
		UE_LOG(LogSound, Warning, TEXT("APXPlayer::PlayThrowPuddingSound|ThrowPuddingSound is nullptr"))
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, ThrowPuddingSound, GetActorLocation());
}

void APXPlayer::PlayLooseLifeSound() const
{
	if (!LooseLifeSound)
	{
		UE_LOG(LogSound, Warning, TEXT("APXPlayer::PlayLooseLifeSound|LooseLifeSound is nullptr"))
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(this, LooseLifeSound, GetActorLocation());
}

void APXPlayer::UpdateFlipbook() const
{
	if (!PXPlayerAppearanceComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::UpdateFlipbook|PXPlayerAppearanceComponent is nullptr"))
		return;
	}

	PXPlayerAppearanceComponent->UpdateFlipbookToForwardVector();
}

void APXPlayer::BindOnShootPuddingAnimationEndDelegate()
{
	if (!PXPlayerAppearanceComponent)
	{
		UE_LOG(LogComponent, Warning,
		       TEXT("APXPlayer::BindOnShootPuddingAnimationEndDelegate|PXPlayerAppearanceComponent is nullptr"))
		return;
	}

	PXPlayerAppearanceComponent->OnShootPuddingAnimationEnd.AddDynamic(this, &APXPlayer::SpawnProjectilePudding);
	PXPlayerAppearanceComponent->OnShootPuddingAnimationEnd.AddDynamic(this, &APXPlayer::ResumeMovement);
}

void APXPlayer::HandleGameOver()
{
	if (!PXPlayerAppearanceComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::GameOver|PXPlayerAppearanceComponent is nullptr"))
		return;
	}
	if (!PXPlayerMovementComponent)
	{
		UE_LOG(LogComponent, Warning, TEXT("APXPlayer::GameOver|PXPlayerMovementComponent is nullptr"))
		return;
	}

	PrintDeathMessage();
	
	PXPlayerAppearanceComponent->SetFlipbookToGameOver();
	PXPlayerMovementComponent->SetCanMove(false);

	OnGameOver.Broadcast();
}

void APXPlayer::UpdateHearts() const
{
	OnPlayerHUDUpdate.Broadcast();
}

void APXPlayer::BecomeUntouchable()
{
	bIsUntouchable = true;

	GetWorld()->GetTimerManager().SetTimer(UntouchableTimerHandle, this, &APXPlayer::BecomeTouchable,
	                                       UntouchableDuration);
}

void APXPlayer::BecomeTouchable()
{
	bIsUntouchable = false;
}
