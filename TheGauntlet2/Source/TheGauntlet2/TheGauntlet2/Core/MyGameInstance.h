#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UENUM(BlueprintType)
enum class EGameState : uint8
{
    MainMenu,
    Gameplay,
    Paused
};

UCLASS()
class THEGAUNTLET2_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    UPROPERTY(BlueprintReadOnly)
    EGameState CurrentState;

    UFUNCTION(BlueprintCallable)
    void StartGame();

    UFUNCTION(BlueprintCallable)
    void ReturnToMainMenu();

    UFUNCTION(BlueprintCallable)
    void SetPaused(bool bPaused);

private:

    void LoadLevel(FName LevelName);
};