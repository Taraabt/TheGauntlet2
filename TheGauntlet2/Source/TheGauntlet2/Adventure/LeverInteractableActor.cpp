#include "Adventure/LeverInteractableActor.h"

#include "Adventure/DefenseTurretActor.h"
#include "Adventure/OrbitingPlatformSystem.h"
#include "Adventure/SimpleDoorActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ALeverInteractableActor::ALeverInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
    LeverMesh->SetupAttachment(SceneRoot);
}

void ALeverInteractableActor::BeginPlay()
{
    Super::BeginPlay();

    DynamicMaterialInstance = LeverMesh->CreateDynamicMaterialInstance(0);
    ApplyLeverColor(InactiveColor);
}

void ALeverInteractableActor::Interact_Implementation(AActor* Interactor)
{
    if (bIsActivated)
    {
        return;
    }

    bIsActivated = true;
    ApplyLeverColor(ActiveColor);

    if (ControlledPlatforms)
    {
        ControlledPlatforms->ActivatePlatforms();
    }

    if (ControlledDoor)
    {
        ControlledDoor->OpenDoor();
    }

    if (ControlledTurret)
    {
        ControlledTurret->DisableTurret(TurretDisabledDuration);
    }
}

void ALeverInteractableActor::ApplyLeverColor(const FLinearColor& NewColor)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetVectorParameterValue(ColorParameterName, NewColor);
    }
}
