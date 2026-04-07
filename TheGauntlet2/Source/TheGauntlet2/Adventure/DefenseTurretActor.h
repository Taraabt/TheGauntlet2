#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefenseTurretActor.generated.h"

class APooledProjectile;
class UMaterialInstanceDynamic;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API ADefenseTurretActor : public AActor
{
    GENERATED_BODY()

public:
    ADefenseTurretActor();

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void DisableTurret(float Duration);

    UFUNCTION(BlueprintCallable, Category = "Turret")
    void EnableTurret();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
    TObjectPtr<UStaticMeshComponent> TurretMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
    TObjectPtr<USceneComponent> MuzzlePoint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
    TSubclassOf<APooledProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (ClampMin = "1"))
    int32 PoolSize = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (ClampMin = "0.01"))
    float FireInterval = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret", meta = (ClampMin = "0.0"))
    float ProjectileSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
    FLinearColor ActiveColor = FLinearColor(1.0f, 0.4f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
    FLinearColor DisabledColor = FLinearColor(0.0f, 0.6f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
    FName ColorParameterName = TEXT("Color");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
    bool bRotateTowardPlayer = true;

private:
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

    UPROPERTY()
    TArray<TObjectPtr<APooledProjectile>> ProjectilePool;

    FTimerHandle FireTimerHandle;
    FTimerHandle ReactivationTimerHandle;
    bool bIsDisabled = false;

    void InitializePool();
    void Fire();
    void StartFiringLoop();
    APooledProjectile* GetAvailableProjectile() const;
    void ApplyTurretColor(const FLinearColor& NewColor);
    FVector GetFireDirection(const FVector& SpawnLocation) const;
};
