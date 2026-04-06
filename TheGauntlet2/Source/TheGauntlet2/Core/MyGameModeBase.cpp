#include "MyGameModeBase.h"

// Engine
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AMyGameModeBase::AMyGameModeBase()
{
	// NON usiamo DefaultPawnClass per evitare problemi
	DefaultPawnClass = nullptr;
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("GAME MODE GAMEPLAY ATTIVA"));

}
