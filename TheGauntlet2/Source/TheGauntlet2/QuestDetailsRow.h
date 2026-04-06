#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestRewardDataAsset.h"
#include "QuestDetailsRow.generated.h"

USTRUCT(BlueprintType)
struct FQuestDetailsRow : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UQuestRewardDataAsset> RewardData;
};
