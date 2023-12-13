// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PacXmas/GameplayElements/Characters/PXCharacter.h"
#include "PXPlayer.generated.h"

class APXProjectilePudding;

UCLASS()
class PACXMAS_API APXPlayer : public APXCharacter
{
	GENERATED_BODY()

public:
	APXPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

public:
	virtual void MoveHorizontal(const float Value) override;
	virtual void MoveVertical(const float Value) override;

	void ChangeLook();

	void CollectMusicSheet();
	void CollectPudding();
	void DropMusicSheet();
	void DropPudding();

	void ShootPudding();

	uint8_t GetLives() const;
	void LoseLive();


private:
	void SpawnPudding();

	FVector LastMoveDirection{FVector::ZeroVector};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APXProjectilePudding> ProjectileClass{nullptr};

	bool bHasPudding{false};
	bool bHasMusicSheet{false};

	uint8_t Lives{3};
	bool bIsInvincible{false};
	FTimerHandle InvincibleTimerHandle;

	bool bIsPlayerInputActive{false};

	UPROPERTY(EditDefaultsOnly, Category="DataAssets|PlayerPudding")
	UPXCharacterDA* PlayerPuddingDA{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="DataAssets|PlayerMusicSheet")
	UPXCharacterDA* PlayerMusicSheetDA{nullptr};
	UPROPERTY(EditDefaultsOnly, Category="DataAssets|PlayerPuddingMusicSheet")
	UPXCharacterDA* PlayerPuddingMusicSheet{nullptr};
};
