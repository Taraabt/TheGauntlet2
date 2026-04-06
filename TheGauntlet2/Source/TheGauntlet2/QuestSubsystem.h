#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "QuestDetailsRow.h"
#include "QuestSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestChangedSignature, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompletedSignature, FName, QuestID);

UCLASS()
class THEGAUNTLET2_API UQuestSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    UFUNCTION(BlueprintCallable, Category = "Quest")
    bool SetActiveQuest(FName QuestID);

    const FQuestDetailsRow* GetQuest(FName QuestID) const;

    const FQuestDetailsRow* GetActiveQuest() const;

    UFUNCTION(BlueprintPure, Category = "Quest")
    FName GetActiveQuestID() const { return ActiveQuestID; }

    UFUNCTION(BlueprintPure, Category = "Quest")
    FText GetActiveQuestTitle() const;

    UFUNCTION(BlueprintPure, Category = "Quest")
    FText GetActiveQuestDescription() const;

    UFUNCTION(BlueprintCallable)
    void CompleteQuest(FName QuestID);

    UPROPERTY(BlueprintAssignable, Category = "Quest")
    FOnQuestChangedSignature OnQuestChanged;

    UPROPERTY(BlueprintAssignable, Category = "Quest")
    FOnQuestCompletedSignature OnQuestCompleted;

private:
    UPROPERTY()
    TObjectPtr<UDataTable> QuestDataTable;

    UPROPERTY()
    FName ActiveQuestID;

    void LoadRewardAsync(TSoftObjectPtr<class UQuestRewardDataAsset> RewardData);
    void PlayRewardEffects(class UQuestRewardDataAsset* RewardDataAsset);
};
