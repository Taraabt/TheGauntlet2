#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class THEGAUNTLET2_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;

private:
    UUserWidget* MainMenuWidget;
};