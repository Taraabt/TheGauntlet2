#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
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
    virtual void Init() override;

    UFUNCTION(BlueprintCallable)
    void StartGame();

    UFUNCTION(BlueprintCallable)
    void ReturnToMainMenu();

    UFUNCTION(BlueprintCallable)
    void SetPaused(bool bPaused);

    void LoadLevel(FName LevelName);

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
    TObjectPtr<UDataTable> QuestDataTable;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
    FName DefaultQuestID;

    UPROPERTY(BlueprintReadOnly)
    EGameState CurrentState = EGameState::MainMenu;
};
