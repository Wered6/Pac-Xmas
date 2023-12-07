// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PacXmas/GameplayElements/Characters/PXCharacter.h"
#include "PXEnemy.generated.h"

class UPXEnemyBehaviorComponent;

UCLASS()
class PACXMAS_API APXEnemy : public APXCharacter
{
	GENERATED_BODY()

public:
	APXEnemy();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FVector GetScaledBoxExtent() const;

	void EatPudding();
	void GetFlashed();

private:
	void StunYourself(const float Time);
	void ResetStun();

	UPROPERTY()
	UPXEnemyBehaviorComponent* BehaviorComponent{nullptr};

	bool bIsStunned{false};
	float EatingPuddingTime{5.f};
	float FlashedTime{2.5f};
};
