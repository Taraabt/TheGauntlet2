#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"
#include "QuestRewardDataAsset.generated.h"

UCLASS(BlueprintType)
class THEGAUNTLET2_API UQuestRewardDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    FVector RewardEffectLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    TSoftObjectPtr<UNiagaraSystem> VictoryVFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    TSoftObjectPtr<USoundBase> VictorySound;
};
