// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "PacXmas/GameplayElements/Items/PXItem.h"
#include "PXMusicSheet.generated.h"

UCLASS()
class PACXMAS_API APXMusicSheet : public APXItem
{
	GENERATED_BODY()

protected:
	virtual void CollectItem(APXPlayer* PXPlayer) override;
};
