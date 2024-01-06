// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PXGameModeMenu.generated.h"

class UPXMusicSubsystem;
class UPXLevelSubsystem;
class UPXMenuManager;

UCLASS()
class PACXMAS_API APXGameModeMenu : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	void OpenAppropriateWidget() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPXMenuManager> PXMenuManagerClass;
	UPROPERTY()
	UPXMenuManager* PXMenuManager{nullptr};

	UPROPERTY()
	UPXLevelSubsystem* PXLevelSubsystem{nullptr};
};
