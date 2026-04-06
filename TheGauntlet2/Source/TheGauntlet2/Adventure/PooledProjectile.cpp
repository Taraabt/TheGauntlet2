#include "Adventure/PooledProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

APooledProjectile::APooledProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    SetRootComponent(CollisionSphere);
    CollisionSphere->SetSphereRadius(16.0f);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Overlap);
    CollisionSphere->SetGenerateOverlapEvents(true);
    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APooledProjectile::OnProjectileOverlap);

    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(CollisionSphere);
    ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 0.0f;
    ProjectileMovement->MaxSpeed = 0.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void APooledProjectile::BeginPlay()
{
    Super::BeginPlay();
    ProjectileMesh->SetRelativeScale3D(VisualScale * MeshScaleMultiplier);
    DeactivateProjectile();
}

void APooledProjectile::ActivateProjectile(const FVector& SpawnLocation, const FVector& Direction, float Speed)
{
    SetActorLocation(SpawnLocation);
    SetActorRotation(Direction.Rotation());
    SetActorScale3D(FVector::OneVector);
    ProjectileMesh->SetRelativeScale3D(VisualScale * MeshScaleMultiplier);

    if (AActor* OwnerActor = GetOwner())
    {
        CollisionSphere->IgnoreActorWhenMoving(OwnerActor, true);
    }

    ProjectileMovement->Velocity = Direction.GetSafeNormal() * Speed;
    ProjectileMovement->Activate();

    SetProjectileActiveState(true);

    if (bDrawDebugPath && GetWorld())
    {
        const FVector DebugEnd = SpawnLocation + (Direction.GetSafeNormal() * Speed * 0.25f);
        DrawDebugLine(GetWorld(), SpawnLocation, DebugEnd, DebugPathColor, false, ProjectileLifetime, 0, 2.0f);
        DrawDebugSphere(GetWorld(), SpawnLocation, 12.0f, 12, DebugPathColor, false, ProjectileLifetime);
    }

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(LifeTimerHandle, this, &APooledProjectile::DeactivateProjectile, ProjectileLifetime, false);
    }
}

void APooledProjectile::DeactivateProjectile()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(LifeTimerHandle);
    }

    ProjectileMovement->StopMovementImmediately();
    ProjectileMovement->Deactivate();
    SetProjectileActiveState(false);
}

void APooledProjectile::OnProjectileOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (!bIsActive)
    {
        return;
    }

    if (OtherActor && OtherActor != this)
    {
        if (OtherActor == GetOwner())
        {
            return;
        }

        DeactivateProjectile();
    }
}

void APooledProjectile::SetProjectileActiveState(bool bNewActive)
{
    bIsActive = bNewActive;
    SetActorHiddenInGame(!bIsActive);
    SetActorEnableCollision(bIsActive);
    CollisionSphere->SetCollisionEnabled(bIsActive ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
    ProjectileMesh->SetHiddenInGame(!bIsActive);
    ProjectileMesh->SetVisibility(bIsActive, true);
}

FVector APooledProjectile::GetCurrentVelocity() const
{
    return ProjectileMovement ? ProjectileMovement->Velocity : FVector::ZeroVector;
}
