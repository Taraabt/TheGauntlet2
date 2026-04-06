#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "ArtifactActor.generated.h"

class UMaterialInstanceDynamic;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API AArtifactActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    AArtifactActor();

    virtual void Interact_Implementation(AActor* Interactor) override;

    UFUNCTION(BlueprintPure, Category = "Artifact")
    bool IsCollected() const { return bCollected; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Artifact")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Artifact")
    TObjectPtr<UStaticMeshComponent> ArtifactMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FVector WorldScale = FVector(1.0f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FVector AttachedScale = FVector(0.4f, 0.4f, 0.4f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FLinearColor AvailableColor = FLinearColor::Yellow;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FLinearColor CollectedColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artifact")
    FName ColorParameterName = TEXT("Color");

private:
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

    bool bCollected = false;
    void ApplyArtifactColor(const FLinearColor& NewColor);
};
