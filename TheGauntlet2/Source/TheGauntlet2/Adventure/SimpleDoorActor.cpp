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

    DoorMesh->SetRelativeLocation(bIsOpen ? OpenRelativeLocation : ClosedRelativeLocation);
}

void ASimpleDoorActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    const FVector TargetLocation = bIsOpen ? OpenRelativeLocation : ClosedRelativeLocation;
    const FVector NewLocation = FMath::VInterpTo(DoorMesh->GetRelativeLocation(), TargetLocation, DeltaSeconds, OpenSpeed);
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
