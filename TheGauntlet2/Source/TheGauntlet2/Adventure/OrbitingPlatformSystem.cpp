#include "Adventure/OrbitingPlatformSystem.h"

#include "Materials/MaterialInterface.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AOrbitingPlatformSystem::AOrbitingPlatformSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    OrbitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("OrbitRoot"));
    OrbitRoot->SetupAttachment(SceneRoot);
}

void AOrbitingPlatformSystem::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    RebuildPlatforms();
}

void AOrbitingPlatformSystem::BeginPlay()
{
    Super::BeginPlay();
    ApplyPlatformColor(bIsActive ? ActiveColor : InactiveColor);
}

void AOrbitingPlatformSystem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsActive && OrbitRoot)
    {
        OrbitRoot->AddLocalRotation(FRotator(0.0f, RotationSpeedDegrees * DeltaSeconds, 0.0f));
    }
}

void AOrbitingPlatformSystem::ActivatePlatforms()
{
    bIsActive = true;
    ApplyPlatformColor(ActiveColor);
}

void AOrbitingPlatformSystem::DeactivatePlatforms()
{
    bIsActive = false;
    ApplyPlatformColor(InactiveColor);
}

void AOrbitingPlatformSystem::RebuildPlatforms()
{
    for (UStaticMeshComponent* PlatformComponent : PlatformComponents)
    {
        if (PlatformComponent)
        {
            PlatformComponent->DestroyComponent();
        }
    }

    PlatformComponents.Empty();
    PlatformMaterials.Empty();

    if (!OrbitRoot || PlatformCount <= 0)
    {
        return;
    }

    for (int32 Index = 0; Index < PlatformCount; ++Index)
    {
        const FName ComponentName = *FString::Printf(TEXT("OrbitPlatform_%d"), Index);
        UStaticMeshComponent* PlatformComponent = NewObject<UStaticMeshComponent>(this, ComponentName);
        if (!PlatformComponent)
        {
            continue;
        }

        AddInstanceComponent(PlatformComponent);
        PlatformComponent->SetupAttachment(OrbitRoot);
        PlatformComponent->SetMobility(EComponentMobility::Movable);
        PlatformComponent->SetStaticMesh(PlatformMesh);
        PlatformComponent->SetRelativeScale3D(PlatformScale);

        if (PlatformMaterialOverride)
        {
            PlatformComponent->SetMaterial(0, PlatformMaterialOverride);
        }

        const float AngleRadians = FMath::DegreesToRadians((360.0f / PlatformCount) * Index);
        const FVector RelativeLocation(
            FMath::Cos(AngleRadians) * OrbitRadius,
            FMath::Sin(AngleRadians) * OrbitRadius,
            0.0f
        );
        PlatformComponent->SetRelativeLocation(RelativeLocation);
        PlatformComponent->RegisterComponent();

        PlatformComponents.Add(PlatformComponent);

        const int32 MaterialSlotCount = PlatformComponent->GetNumMaterials();
        for (int32 MaterialIndex = 0; MaterialIndex < MaterialSlotCount; ++MaterialIndex)
        {
            if (PlatformMaterialOverride)
            {
                PlatformComponent->SetMaterial(MaterialIndex, PlatformMaterialOverride);
            }

            if (UMaterialInstanceDynamic* DynamicMaterial = PlatformComponent->CreateDynamicMaterialInstance(MaterialIndex))
            {
                PlatformMaterials.Add(DynamicMaterial);
            }
        }
    }

    ApplyPlatformColor(bIsActive ? ActiveColor : InactiveColor);
}

void AOrbitingPlatformSystem::ApplyPlatformColor(const FLinearColor& NewColor)
{
    for (UMaterialInstanceDynamic* PlatformMaterial : PlatformMaterials)
    {
        if (PlatformMaterial)
        {
            PlatformMaterial->SetVectorParameterValue(ColorParameterName, NewColor);
        }
    }
}
