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
        UE_LOG(LogTemp, Error, TEXT("GameInstance non valida"));
        return;
    }

    QuestDataTable = GI->QuestDataTable;

    if (!QuestDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestDataTable NON assegnata"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestSubsystem inizializzato"));
    }

    if (!GI->DefaultQuestID.IsNone())
    {
        SetActiveQuest(GI->DefaultQuestID);
    }
}

bool UQuestSubsystem::SetActiveQuest(FName QuestID)
{
    if (QuestID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("SetActiveQuest chiamata con QuestID non valido"));
        return false;
    }

    const FQuestDetailsRow* QuestRow = GetQuest(QuestID);
    if (!QuestRow)
    {
        UE_LOG(LogTemp, Warning, TEXT("Impossibile attivare la quest '%s': riga non trovata"), *QuestID.ToString());
        return false;
    }

    ActiveQuestID = QuestID;
    OnQuestChanged.Broadcast(ActiveQuestID);

    UE_LOG(LogTemp, Warning, TEXT("Quest attiva impostata: %s"), *ActiveQuestID.ToString());
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
        UE_LOG(LogTemp, Warning, TEXT("Quest NON trovata"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Quest completata: %s"), *QuestID.ToString());

    if (QuestID == ActiveQuestID)
    {
        OnQuestCompleted.Broadcast(QuestID);
    }

    if (Quest->RewardData.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("Quest '%s': RewardData non assegnata"), *QuestID.ToString());
    }

    LoadRewardAsync(Quest->RewardData);
}

void UQuestSubsystem::LoadRewardAsync(TSoftObjectPtr<UQuestRewardDataAsset> RewardData)
{
    if (RewardData.IsNull())
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadRewardAsync chiamata con RewardData nulla"));
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
                UE_LOG(LogTemp, Warning, TEXT("RewardData non caricata correttamente"));
                return;
            }

            UE_LOG(LogTemp, Warning, TEXT("Reward caricata!"));
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
        UE_LOG(LogTemp, Warning, TEXT("La reward non contiene ne' VFX ne' Sound"));
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
                UE_LOG(LogTemp, Warning, TEXT("Spawn del VFX di vittoria"));
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                    CurrentWorld,
                    VFX,
                    RewardDataAsset->RewardEffectLocation
                );
            }
            else if (!RewardDataAsset->VictoryVFX.IsNull())
            {
                UE_LOG(LogTemp, Warning, TEXT("VictoryVFX assegnato ma non caricato correttamente"));
            }

            if (USoundBase* Sound = RewardDataAsset->VictorySound.Get())
            {
                UE_LOG(LogTemp, Warning, TEXT("Play del Sound di vittoria"));
                UGameplayStatics::PlaySound2D(CurrentWorld, Sound);
            }
            else if (!RewardDataAsset->VictorySound.IsNull())
            {
                UE_LOG(LogTemp, Warning, TEXT("VictorySound assegnato ma non caricato correttamente"));
            }
        }
    );
}
