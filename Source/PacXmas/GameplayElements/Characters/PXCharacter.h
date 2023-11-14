// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PXCharacter.generated.h"

class UPaperFlipbookComponent;
class UFloatingPawnMovement;
class UBoxComponent;
class UPXCharacterDA;

UCLASS()
class PACXMAS_API APXCharacter : public APawn
{
	GENERATED_BODY()

public:
	APXCharacter();

protected:
	virtual void BeginPlay() override;

	void SetFlipbookToIdle();

	UPROPERTY(EditAnywhere, Category="DataAsset")
	UPXCharacterDA* CharacterDA{nullptr};

public:
	virtual void Tick(float DeltaTime) override;

	virtual void MoveHorizontal(const float Value);
	virtual void MoveVertical(const float Value);

private:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionComp{nullptr};
	UPROPERTY()
	UFloatingPawnMovement* FloatingPawnMovement{nullptr};
	UPROPERTY(EditDefaultsOnly)
	UPaperFlipbookComponent* FlipbookComp{nullptr};

	const float CollisionWidth{31.f};
	const float CollisionDepth{31.f};
	const float CollisionHeight{31.f};
};