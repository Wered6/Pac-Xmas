// Copyright (c) 2023 Smoking Carrots. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PXChooseClassWidget.generated.h"

class UPXClassSubsystem;

UCLASS()
class PACXMAS_API UPXChooseClassWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void PickGirl() const;
	UFUNCTION(BlueprintCallable)
	void PickBoy() const;

private:
	void InitializeClassSubsystem();

	UPROPERTY()
	UPXClassSubsystem* PXClassSubsystem{nullptr};
};
