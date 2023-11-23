// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PXItemDA.generated.h"

class UPaperSprite;

UCLASS()
class PACXMAS_API UPXItemDA : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sprites")
	UPaperSprite* SpriteComponent{nullptr};
};
