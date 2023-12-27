// Copyright (c) 2023 Santa Claus. All rights reserved.


#include "PXGameModeGameplay.h"
#include "PacXmas/GameInstance/PXGameInstance.h"
#include "PacXmas/GameplayElements/Characters/Player/PXPlayer.h"
#include "PacXmas/GameplayElements/Items/MusicSheet/PXMusicSheet.h"
#include "PacXmas/GameplayElements/Items/Pudding/PXPudding.h"
#include "PacXmas/Subsystems/SpawnItemsSubsystem/PXSpawnItemsSubsystem.h"
#include "PacXmas/Utilities/CustomLogs/PXCustomLogs.h"

void APXGameModeGameplay::BeginPlay()
{
	Super::BeginPlay();

	InitializePXSpawnItemsSubsystem();

	SpawnPudding();
	SpawnMusicSheet();
}

void APXGameModeGameplay::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	if (!NewPlayer)
	{
		UE_LOG(LogController, Warning, TEXT("APXGameModeGameplay::RestartPlayerAtPlayerStart|NewPlayer is nullptr"))
		return;
	}
	if (!StartSpot)
	{
		UE_LOG(LogActor, Warning, TEXT("APXGameModeGameplay::RestartPlayerAtPlayerStart|StartSport is nullptr"))
		return;
	}

	// Determine the class to spawn
	UClass* PlayerClassToSpawn = GetPlayerClass();

	if (!PlayerClassToSpawn)
	{
		UE_LOG(LogClass, Warning, TEXT("APXGameModeGameplay::RestartPlayerAtPlayerStart|PlayerClassToSpawn is nullptr"))
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.bNoFail = true;
	SpawnParameters.Owner = NewPlayer;

	APXPlayer* PXPlayer = GetWorld()->SpawnActor<APXPlayer>(PlayerClassToSpawn, StartSpot->GetActorLocation(),
	                                                        StartSpot->GetActorRotation(), SpawnParameters);

	if (!PXPlayer)
	{
		UE_LOG(LogActor, Warning, TEXT("APXGameModeGameplay::RestartPlayerAtPlayerStart|PXPlayer is nullptr"))
		return;
	}

	NewPlayer->Possess(PXPlayer);
}

void APXGameModeGameplay::SpawnPudding(const float SpawnDelay) const
{
	if (!PXSpawnItemsSubsystem)
	{
		UE_LOG(LogSubsystem, Warning, TEXT("APXGameModeGameplay::SpawnPudding|PXSpawnItemsSubsystem is nullptr"))
		return;
	}
	if (!PuddingClass)
	{
		UE_LOG(LogClass, Warning, TEXT("APXGameModeGameplay::SpawnPudding|PuddingClass is nullptr"))
		return;
	}

	PXSpawnItemsSubsystem->SpawnPudding(PuddingClass, SpawnDelay);
}

void APXGameModeGameplay::SpawnMusicSheet() const
{
	if (!PXSpawnItemsSubsystem)
	{
		UE_LOG(LogSubsystem, Warning, TEXT("APXGameModeGameplay::SpawnMusicSheet|PXSpawnItemsSubsystem is nullptr"))
		return;
	}
	if (!MusicSheetClass)
	{
		UE_LOG(LogClass, Warning, TEXT("APXGameModeGameplay::SpawnMusicSheet|MusicSheetClass is nullptr"))
		return;
	}

	PXSpawnItemsSubsystem->SpawnMusicSheet(MusicSheetClass);
}

TSubclassOf<APXPlayer> APXGameModeGameplay::GetPlayerClass() const
{
	const UPXGameInstance* PXGameInstance = Cast<UPXGameInstance>(GetGameInstance());

	if (!PXGameInstance)
	{
		UE_LOG(LogGameInstance, Warning, TEXT("APXGameModeGameplay::GetPlayerClass"))
		return nullptr;
	}
	if (!BoyClass)
	{
		UE_LOG(LogClass, Warning, TEXT("APXGameModeGameplay::GetPlayerClass|BoyClass is nullptr"))
		return nullptr;
	}
	if (!GirlClass)
	{
		UE_LOG(LogClass, Warning, TEXT("APXGameModeGameplay::GetPlayerClass|GirlsClass is nullptr"))
		return nullptr;
	}

	const EPlayerClass PlayerClass = PXGameInstance->GetPlayerClass();

	switch (PlayerClass)
	{
	case EPlayerClass::Boy:
		return BoyClass;
	case EPlayerClass::Girl:
		return GirlClass;
	}
	return nullptr;
}

void APXGameModeGameplay::InitializePXSpawnItemsSubsystem()
{
	PXSpawnItemsSubsystem = GetWorld()->GetSubsystem<UPXSpawnItemsSubsystem>();
}
