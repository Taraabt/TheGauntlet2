#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledProjectile.generated.h"

class UPrimitiveComponent;
class UProjectileMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API APooledProjectile : public AActor
{
    GENERATED_BODY()

public:
    APooledProjectile();

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void ActivateProjectile(const FVector& SpawnLocation, const FVector& Direction, float Speed);

    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void DeactivateProjectile();

    UFUNCTION(BlueprintPure, Category = "Projectile")
    bool IsProjectileActive() const { return bIsActive; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<USphereComponent> CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UStaticMeshComponent> ProjectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (ClampMin = "0.1"))
    float ProjectileLifetime = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    FVector VisualScale = FVector(0.2f, 0.2f, 0.2f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
    float MeshScaleMultiplier = 0.2f;

private:
    UFUNCTION()
    void OnProjectileHit(
        UPrimitiveComponent* HitComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit
    );

    UFUNCTION()
    void OnProjectileOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void SetProjectileActiveState(bool bNewActive);

    bool bIsActive = false;
    FTimerHandle LifeTimerHandle;
};
