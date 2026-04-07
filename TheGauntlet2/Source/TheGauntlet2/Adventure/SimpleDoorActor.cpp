#include "Adventure/SimpleDoorActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ASimpleDoorActor::ASimpleDoorActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(SceneRoot);
    DoorMesh->SetMobility(EComponentMobility::Movable);
}

void ASimpleDoorActor::BeginPlay()
{
    Super::BeginPlay();

    ClosedRelativeLocation = DoorMesh->GetRelativeLocation();
    OpenRelativeLocation = ClosedRelativeLocation + OpenOffset;
    bIsOpen = bStartsOpen;
    CurrentMoveAlpha = bIsOpen ? 1.0f : 0.0f;

    DoorMesh->SetRelativeLocation(bIsOpen ? OpenRelativeLocation : ClosedRelativeLocation);
}

void ASimpleDoorActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (OpenDuration <= KINDA_SMALL_NUMBER)
    {
        DoorMesh->SetRelativeLocation(bIsOpen ? OpenRelativeLocation : ClosedRelativeLocation);
        CurrentMoveAlpha = bIsOpen ? 1.0f : 0.0f;
        return;
    }

    const float AlphaDirection = bIsOpen ? 1.0f : -1.0f;
    CurrentMoveAlpha = FMath::Clamp(CurrentMoveAlpha + ((DeltaSeconds / OpenDuration) * AlphaDirection), 0.0f, 1.0f);
    const FVector NewLocation = FMath::Lerp(ClosedRelativeLocation, OpenRelativeLocation, CurrentMoveAlpha);
    DoorMesh->SetRelativeLocation(NewLocation);
}

void ASimpleDoorActor::OpenDoor()
{
    bIsOpen = true;
}

void ASimpleDoorActor::CloseDoor()
{
    bIsOpen = false;
}
