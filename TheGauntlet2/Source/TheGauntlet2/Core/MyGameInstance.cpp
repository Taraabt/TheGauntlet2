#include "Core/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UMyGameInstance::Init()
{
    Super::Init();
}
void UMyGameInstance::StartGame()
{
    CurrentState = EGameState::Gameplay;

    LoadLevel("LV_Gameplay");
}
void UMyGameInstance::ReturnToMainMenu()
{
    CurrentState = EGameState::MainMenu;

    LoadLevel("LV_MainMenu");
}
void UMyGameInstance::SetPaused(bool bPaused)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    UGameplayStatics::SetGamePaused(World, bPaused);

    CurrentState = bPaused ? EGameState::Paused : EGameState::Gameplay;
}
void UMyGameInstance::LoadLevel(FName LevelName)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        return;
    }

    UGameplayStatics::SetGamePaused(World, false);

    UGameplayStatics::OpenLevel(World, LevelName);
}
