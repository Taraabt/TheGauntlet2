#include "UI/QuestHUDWidget.h"

#include "QuestSubsystem.h"

FText UQuestHUDWidget::GetCurrentQuestTitle() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>())
        {
            return QuestSubsystem->GetActiveQuestTitle();
        }
    }

    return FText::GetEmpty();
}

FText UQuestHUDWidget::GetCurrentQuestDescription() const
{
    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>())
        {
            return QuestSubsystem->GetActiveQuestDescription();
        }
    }

    return FText::GetEmpty();
}
