#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestHUDWidget.generated.h"

UCLASS()
class THEGAUNTLET2_API UQuestHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "Quest")
    FText GetCurrentQuestTitle() const;

    UFUNCTION(BlueprintPure, Category = "Quest")
    FText GetCurrentQuestDescription() const;
};
