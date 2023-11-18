// Copyright (c) 2023 Santa Claus. All rights reserved.

#include "PXEnemyBehaviorComponent.h"
#include "PacXmas/GameplayElements/Characters/Enemies/PXEnemy.h"
#include "PacXmas/Utilities/CustomLogs/PXCustomLogs.h"

template <typename T>
void ShuffleArray(TArray<T>& Array)
{
	for (int32 Index = Array.Num() - 1; Index > 0; --Index)
	{
		int32 SwapIndex = FMath::RandRange(0, Index);
		Array.Swap(Index, SwapIndex);
	}
}

UPXEnemyBehaviorComponent::UPXEnemyBehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CurrentDirection = FVector::UpVector;
}

void UPXEnemyBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner())
	{
		UE_LOG(LogComponent, Warning, TEXT("UPXEnemyBehaviorComponent::BeginPlay|Owner is nullptr"))
		return;
	}
	BaseLocation = GetOwner()->GetActorLocation();
}

void UPXEnemyBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DirectionChangeCooldown > 0)
	{
		DirectionChangeCooldown--;
	}
}

FVector UPXEnemyBehaviorComponent::DetermineNextDirection()
{
	const APXEnemy* PXEnemy = Cast<APXEnemy>(GetOwner());
	if (!PXEnemy)
	{
		return CurrentDirection;
	}

	if (!CanMoveInAxis(FVector::ForwardVector) && !CanMoveInAxis(FVector::UpVector))
	{
		return CurrentDirection;
	}

	TArray<FVector> PossibleDirections = FilterPossibleDirections();
	PossibleDirections.Remove(-CurrentDirection);
	ShuffleArray(PossibleDirections);

	FVector SelectedDirection{CurrentDirection};
	if (DirectionChangeCooldown <= 0)
	{
		const FVector NewDirection = SelectDirectionFromPossibilities(PossibleDirections);
		if (NewDirection != CurrentDirection)
		{
			SelectedDirection = NewDirection;
			DirectionChangeCooldown = MaxCooldown;
		}
	}

	CurrentDirection = SelectedDirection;
	return CurrentDirection;
}

bool UPXEnemyBehaviorComponent::CanMoveInDirection(const FVector& Direction) const
{
	const APXEnemy* PXEnemy = Cast<APXEnemy>(GetOwner());
	if (PXEnemy)
	{
		return PXEnemy->CanMoveInDirection(Direction, MoveCheckDistance);
	}

	return false;
}

bool UPXEnemyBehaviorComponent::CanMoveInAxis(const FVector& Axis) const
{
	float CurrentLocationAxis;

	if (Axis.X != 0.f)
	{
		CurrentLocationAxis = FMath::Abs(BaseLocation.X - GetOwner()->GetActorLocation().X);
	}
	else
	{
		CurrentLocationAxis = FMath::Abs(BaseLocation.Z - GetOwner()->GetActorLocation().Z);
	}

	constexpr float RightOffset{0.5f};
	constexpr float LeftOffset{31.5f};

	return FMath::Fmod(CurrentLocationAxis, LocationOffset) < RightOffset ||
		FMath::Fmod(CurrentLocationAxis, LocationOffset) > LeftOffset;
}

TArray<FVector> UPXEnemyBehaviorComponent::FilterPossibleDirections() const
{
	TArray<FVector> PossibleDirections =
	{
		FVector::UpVector,
		FVector::ForwardVector,
		FVector::DownVector,
		FVector::BackwardVector
	};

	if (!CanMoveInAxis(FVector::UpVector))
	{
		PossibleDirections.Remove(FVector::BackwardVector);
		PossibleDirections.Remove(FVector::ForwardVector);
	}
	if (!CanMoveInAxis(FVector::ForwardVector))
	{
		PossibleDirections.Remove(FVector::UpVector);
		PossibleDirections.Remove(FVector::DownVector);
	}

	return PossibleDirections;
}

FVector UPXEnemyBehaviorComponent::SelectDirectionFromPossibilities(const TArray<FVector>& Directions) const
{
	for (const FVector& Direction : Directions)
	{
		if (CanMoveInDirection(Direction) && (Direction == CurrentDirection || FMath::RandRange(0.f, 1.f) <
			TurnProbability))
		{
			return Direction;
		}
	}
	return -CurrentDirection;
}
