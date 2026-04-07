#include "QuestSubsystem.h"
#include "Core/MyGameInstance.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "QuestRewardDataAsset.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());

    if (!GI)
    {
        return;
    }

    QuestDataTable = GI->QuestDataTable;

    if (!GI->DefaultQuestID.IsNone())
    {
        SetActiveQuest(GI->DefaultQuestID);
    }
}

bool UQuestSubsystem::SetActiveQuest(FName QuestID)
{
    if (QuestID.IsNone())
    {
        return false;
    }

    const FQuestDetailsRow* QuestRow = GetQuest(QuestID);
    if (!QuestRow)
    {
        return false;
    }

    ActiveQuestID = QuestID;
    OnQuestChanged.Broadcast(ActiveQuestID);
    return true;
}

const FQuestDetailsRow* UQuestSubsystem::GetQuest(FName QuestID) const
{
    if (!QuestDataTable) return nullptr;

    return QuestDataTable->FindRow<FQuestDetailsRow>(QuestID, TEXT(""));
}

const FQuestDetailsRow* UQuestSubsystem::GetActiveQuest() const
{
    return GetQuest(ActiveQuestID);
}

FText UQuestSubsystem::GetActiveQuestTitle() const
{
    const FQuestDetailsRow* Quest = GetActiveQuest();
    return Quest ? Quest->Title : FText::GetEmpty();
}

FText UQuestSubsystem::GetActiveQuestDescription() const
{
    const FQuestDetailsRow* Quest = GetActiveQuest();
    return Quest ? Quest->Description : FText::GetEmpty();
}

void UQuestSubsystem::CompleteQuest(FName QuestID)
{
    const FQuestDetailsRow* Quest = GetQuest(QuestID);

    if (!Quest)
    {
        return;
    }

    if (QuestID == ActiveQuestID)
    {
        OnQuestCompleted.Broadcast(QuestID);
    }

    LoadRewardAsync(Quest->RewardData);
}

void UQuestSubsystem::LoadRewardAsync(TSoftObjectPtr<UQuestRewardDataAsset> RewardData)
{
    if (RewardData.IsNull())
    {
        return;
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

    Streamable.RequestAsyncLoad(
        RewardData.ToSoftObjectPath(),
        [RewardData, this]()
        {
            UQuestRewardDataAsset* Data = RewardData.Get();

            if (!Data)
            {
                return;
            }

            PlayRewardEffects(Data);
        }
    );
}

void UQuestSubsystem::PlayRewardEffects(UQuestRewardDataAsset* RewardDataAsset)
{
    if (!RewardDataAsset)
    {
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    TArray<FSoftObjectPath> AssetsToLoad;
    if (!RewardDataAsset->VictoryVFX.IsNull())
    {
        AssetsToLoad.Add(RewardDataAsset->VictoryVFX.ToSoftObjectPath());
    }

    if (!RewardDataAsset->VictorySound.IsNull())
    {
        AssetsToLoad.Add(RewardDataAsset->VictorySound.ToSoftObjectPath());
    }

    if (AssetsToLoad.Num() == 0)
    {
        return;
    }

    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    Streamable.RequestAsyncLoad(
        AssetsToLoad,
        [this, RewardDataAsset]()
        {
            UWorld* CurrentWorld = GetWorld();
            if (!CurrentWorld)
            {
                return;
            }

            if (UNiagaraSystem* VFX = RewardDataAsset->VictoryVFX.Get())
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    CurrentWorld,
                    VFX,
                    RewardDataAsset->RewardEffectLocation
                );
            }

            if (USoundBase* Sound = RewardDataAsset->VictorySound.Get())
            {
                UGameplayStatics::PlaySound2D(CurrentWorld, Sound);
            }
        }
    );
}
