// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PXItem.generated.h"

class UBoxComponent;
class UPaperSpriteComponent;
class UPXItemDA;

UCLASS()
class PACXMAS_API APXItem : public AActor
{
	GENERATED_BODY()

public:
	APXItem();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionComponent{nullptr};

private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	UPXItemDA* ItemDA{nullptr};
	UPROPERTY(EditDefaultsOnly)
	UPaperSpriteComponent* PaperSpriteComponent{nullptr};

	const float CollisionWidth{31.f};
	const float CollisionDepth{31.f};
	const float CollisionHeight{31.f};
};
