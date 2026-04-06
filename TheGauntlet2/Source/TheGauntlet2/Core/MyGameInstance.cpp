#include "Core/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void UMyGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Warning, TEXT("GameInstance Init"));

    if (!QuestDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT(" QuestDataTable NON assegnata!"));
    }
}
void UMyGameInstance::StartGame()
{
    UE_LOG(LogTemp, Warning, TEXT("StartGame chiamato"));

    CurrentState = EGameState::Gameplay;

    LoadLevel("LV_Gameplay");
}
void UMyGameInstance::ReturnToMainMenu()
{
    UE_LOG(LogTemp, Warning, TEXT("ReturnToMainMenu chiamato"));

    CurrentState = EGameState::MainMenu;

    LoadLevel("LV_MainMenu");
}
void UMyGameInstance::SetPaused(bool bPaused)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World NULL"));
        return;
    }

    UGameplayStatics::SetGamePaused(World, bPaused);

    CurrentState = bPaused ? EGameState::Paused : EGameState::Gameplay;

    UE_LOG(LogTemp, Warning, TEXT("Paused: %s"), bPaused ? TEXT("TRUE") : TEXT("FALSE"));
}
void UMyGameInstance::LoadLevel(FName LevelName)
{
    UE_LOG(LogTemp, Warning, TEXT("Loading Level: %s"), *LevelName.ToString());

    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World NULL nella GameInstance"));
        return;
    }

    UGameplayStatics::SetGamePaused(World, false);

    UGameplayStatics::OpenLevel(World, LevelName);
}
