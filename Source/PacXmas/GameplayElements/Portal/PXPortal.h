// Copyright (c) 2023 Santa Claus. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PXPortal.generated.h"

class UBoxComponent;

UCLASS()
class PACXMAS_API APXPortal : public AActor
{
	GENERATED_BODY()

public:
	APXPortal();

protected:
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                            const FHitResult& SweepResult);

private:
	UPROPERTY(EditInstanceOnly)
	FVector TargetDestination;

	UPROPERTY()
	UBoxComponent* CollisionComponent{nullptr};

	const float CollisionSize{31.f};
};
