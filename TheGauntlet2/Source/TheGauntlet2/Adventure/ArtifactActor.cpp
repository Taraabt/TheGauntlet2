#include "Adventure/ArtifactActor.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TheGauntlet2Character.h"

AArtifactActor::AArtifactActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    ArtifactMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArtifactMesh"));
    ArtifactMesh->SetupAttachment(SceneRoot);
    ArtifactMesh->SetSimulatePhysics(true);
    ArtifactMesh->SetRelativeScale3D(WorldScale);
}

void AArtifactActor::BeginPlay()
{
    Super::BeginPlay();

    ArtifactMesh->SetRelativeScale3D(WorldScale);
    DynamicMaterialInstance = ArtifactMesh->CreateDynamicMaterialInstance(0);
    ApplyArtifactColor(AvailableColor);
}

void AArtifactActor::Interact_Implementation(AActor* Interactor)
{
    if (bCollected)
    {
        return;
    }

    ATheGauntlet2Character* Character = Cast<ATheGauntlet2Character>(Interactor);
    if (!Character)
    {
        return;
    }

    USkeletalMeshComponent* AttachMesh = Character->GetArtifactAttachMesh();
    if (!AttachMesh)
    {
        return;
    }

    if (!AttachMesh->DoesSocketExist(Character->GetArtifactSocketName()))
    {
        return;
    }

    bCollected = true;

    ArtifactMesh->SetSimulatePhysics(false);
    ArtifactMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SetActorEnableCollision(false);
    ArtifactMesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    ArtifactMesh->AttachToComponent(
        AttachMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        Character->GetArtifactSocketName()
    );
    ArtifactMesh->SetRelativeLocation(FVector::ZeroVector);
    ArtifactMesh->SetRelativeRotation(FRotator::ZeroRotator);
    ArtifactMesh->SetRelativeScale3D(AttachedScale);
    ArtifactMesh->SetHiddenInGame(false);

    Character->SetHasArtifact(true);
    Character->SetCarriedArtifact(this);
    ApplyArtifactColor(CollectedColor);
}

void AArtifactActor::ApplyArtifactColor(const FLinearColor& NewColor)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetVectorParameterValue(ColorParameterName, NewColor);
    }
}
