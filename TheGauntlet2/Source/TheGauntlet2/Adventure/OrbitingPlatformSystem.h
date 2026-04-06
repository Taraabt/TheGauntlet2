#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitingPlatformSystem.generated.h"

class UMaterialInstanceDynamic;
class UMaterialInterface;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API AOrbitingPlatformSystem : public AActor
{
    GENERATED_BODY()

public:
    AOrbitingPlatformSystem();

    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Platforms")
    void ActivatePlatforms();

    UFUNCTION(BlueprintCallable, Category = "Platforms")
    void DeactivatePlatforms();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platforms")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Platforms")
    TObjectPtr<USceneComponent> OrbitRoot;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    TObjectPtr<UStaticMesh> PlatformMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    TObjectPtr<UMaterialInterface> PlatformMaterialOverride;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms", meta = (ClampMin = "1"))
    int32 PlatformCount = 4;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms", meta = (ClampMin = "0.0"))
    float OrbitRadius = 300.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    FVector PlatformScale = FVector(1.5f, 1.5f, 0.35f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    FLinearColor InactiveColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    FLinearColor ActiveColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms")
    FName ColorParameterName = TEXT("Color");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Platforms", meta = (ClampMin = "0.0"))
    float RotationSpeedDegrees = 45.0f;

private:
    UPROPERTY()
    TArray<TObjectPtr<UStaticMeshComponent>> PlatformComponents;

    UPROPERTY()
    TArray<TObjectPtr<UMaterialInstanceDynamic>> PlatformMaterials;

    bool bIsActive = false;

    void RebuildPlatforms();
    void ApplyPlatformColor(const FLinearColor& NewColor);
};
