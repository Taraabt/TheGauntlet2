#include "Adventure/ExitActor.h"

#include "Adventure/ArtifactActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/MyGameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "QuestSubsystem.h"
#include "TheGauntlet2Character.h"
#include "TimerManager.h"

AExitActor::AExitActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    ExitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ExitMesh"));
    ExitMesh->SetupAttachment(SceneRoot);

    ArtifactPlacementPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ArtifactPlacementPoint"));
    ArtifactPlacementPoint->SetupAttachment(SceneRoot);
}

void AExitActor::BeginPlay()
{
    Super::BeginPlay();

    DynamicMaterialInstance = ExitMesh->CreateDynamicMaterialInstance(0);
    ApplyExitColor(LockedColor);
}

void AExitActor::Interact_Implementation(AActor* Interactor)
{
    if (bMissionCompleted)
    {
        return;
    }

    ATheGauntlet2Character* Character = Cast<ATheGauntlet2Character>(Interactor);
    if (!Character || !Character->HasArtifact())
    {
        return;
    }

    AActor* CarriedArtifact = Character->GetCarriedArtifact();
    if (CarriedArtifact)
    {
        // Place the artifact on the altar before completing the quest.
        CarriedArtifact->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CarriedArtifact->AttachToComponent(
            ArtifactPlacementPoint,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale
        );
    }

    Character->SetHasArtifact(false);
    Character->SetCarriedArtifact(nullptr);

    if (UGameInstance* GameInstance = GetGameInstance())
    {
        if (UQuestSubsystem* QuestSubsystem = GameInstance->GetSubsystem<UQuestSubsystem>())
        {
            QuestSubsystem->CompleteQuest(QuestID);
        }
    }

    bMissionCompleted = true;
    ApplyExitColor(CompletedColor);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(ReturnTimerHandle, this, &AExitActor::ReturnToMainMenu, ReturnToMenuDelay, false);
    }
}

void AExitActor::ApplyExitColor(const FLinearColor& NewColor)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetVectorParameterValue(ColorParameterName, NewColor);
    }
}

void AExitActor::ReturnToMainMenu()
{
    if (UMyGameInstance* GameInstance = GetGameInstance<UMyGameInstance>())
    {
        GameInstance->ReturnToMainMenu();
    }
}
