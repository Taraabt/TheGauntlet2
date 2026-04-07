#include "MyGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = nullptr;
}

void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}
