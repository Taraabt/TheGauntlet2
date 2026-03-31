#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

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
    if (bPaused)
    {
        CurrentState = EGameState::Paused;
        UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
    else
    {
        CurrentState = EGameState::Gameplay;
        UGameplayStatics::SetGamePaused(GetWorld(), false);
    }
}

void UMyGameInstance::LoadLevel(FName LevelName)
{
    UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}