#include "Adventure/DefenseTurretActor.h"

#include "Adventure/PooledProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

ADefenseTurretActor::ADefenseTurretActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    TurretMesh->SetupAttachment(SceneRoot);

    MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
    MuzzlePoint->SetupAttachment(TurretMesh);
}

void ADefenseTurretActor::BeginPlay()
{
    Super::BeginPlay();

    DynamicMaterialInstance = TurretMesh->CreateDynamicMaterialInstance(0);
    ApplyTurretColor(ActiveColor);

    InitializePool();
    StartFiringLoop();
}

void ADefenseTurretActor::DisableTurret(float Duration)
{
    bIsDisabled = true;
    ApplyTurretColor(DisabledColor);

    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(FireTimerHandle);
        World->GetTimerManager().ClearTimer(ReactivationTimerHandle);
        World->GetTimerManager().SetTimer(ReactivationTimerHandle, this, &ADefenseTurretActor::EnableTurret, Duration, false);
    }

    DisplayPoolDebug();
}

void ADefenseTurretActor::EnableTurret()
{
    bIsDisabled = false;
    ApplyTurretColor(ActiveColor);
    StartFiringLoop();
    DisplayPoolDebug();
}

void ADefenseTurretActor::InitializePool()
{
    if (!ProjectileClass || !GetWorld())
    {
        UE_LOG(LogTemp, Warning, TEXT("DefenseTurretActor '%s': ProjectileClass non assegnata o World non valida"), *GetName());
        return;
    }

    // The turret creates every projectile once and reuses them during gameplay.
    ProjectilePool.Reserve(PoolSize);
    for (int32 Index = 0; Index < PoolSize; ++Index)
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = this;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        APooledProjectile* Projectile = GetWorld()->SpawnActor<APooledProjectile>(
            ProjectileClass,
            GetActorLocation(),
            GetActorRotation(),
            SpawnParameters
        );
        if (Projectile)
        {
            Projectile->DeactivateProjectile();
            ProjectilePool.Add(Projectile);
        }
    }

    DisplayPoolDebug();
}

void ADefenseTurretActor::Fire()
{
    if (bIsDisabled)
    {
        return;
    }

    APooledProjectile* Projectile = GetAvailableProjectile();
    if (!Projectile)
    {
        UE_LOG(LogTemp, Verbose, TEXT("DefenseTurretActor '%s': nessun proiettile disponibile nel pool"), *GetName());
        DisplayPoolDebug();
        return;
    }

    const FVector SpawnLocation = MuzzlePoint ? MuzzlePoint->GetComponentLocation() : GetActorLocation();
    const FVector FireDirection = GetFireDirection(SpawnLocation);

    Projectile->ActivateProjectile(SpawnLocation, FireDirection, ProjectileSpeed);

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Turret '%s' fired projectile '%s' from %s with velocity %s"),
        *GetName(),
        *Projectile->GetName(),
        *SpawnLocation.ToString(),
        *Projectile->GetCurrentVelocity().ToString()
    );

    DisplayPoolDebug();
}

void ADefenseTurretActor::StartFiringLoop()
{
    if (!GetWorld())
    {
        return;
    }

    GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &ADefenseTurretActor::Fire, FireInterval, true);
}

APooledProjectile* ADefenseTurretActor::GetAvailableProjectile() const
{
    for (APooledProjectile* Projectile : ProjectilePool)
    {
        if (Projectile && !Projectile->IsProjectileActive())
        {
            return Projectile;
        }
    }

    return nullptr;
}

int32 ADefenseTurretActor::CountActiveProjectiles() const
{
    int32 ActiveProjectiles = 0;
    for (const APooledProjectile* Projectile : ProjectilePool)
    {
        if (Projectile && Projectile->IsProjectileActive())
        {
            ++ActiveProjectiles;
        }
    }

    return ActiveProjectiles;
}

void ADefenseTurretActor::DisplayPoolDebug() const
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            reinterpret_cast<uint64>(this),
            1.0f,
            FColor::White,
            FString::Printf(TEXT("Pool Size: %d | Active: %d"), ProjectilePool.Num(), CountActiveProjectiles())
        );
    }
}

void ADefenseTurretActor::ApplyTurretColor(const FLinearColor& NewColor)
{
    if (DynamicMaterialInstance)
    {
        DynamicMaterialInstance->SetVectorParameterValue(ColorParameterName, NewColor);
    }
}

FVector ADefenseTurretActor::GetFireDirection(const FVector& SpawnLocation) const
{
    FVector FireDirection = MuzzlePoint ? MuzzlePoint->GetForwardVector() : GetActorForwardVector();

    if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        PlayerLocation.Z = SpawnLocation.Z;
        FireDirection = (PlayerLocation - SpawnLocation).GetSafeNormal();

        if (bRotateTowardPlayer)
        {
            const FRotator LookAtRotation = FireDirection.Rotation();

            if (MuzzlePoint)
            {
                MuzzlePoint->SetWorldRotation(LookAtRotation);
            }

            if (TurretMesh)
            {
                TurretMesh->SetWorldRotation(LookAtRotation);
            }
        }
    }

    return FireDirection.GetSafeNormal();
}
