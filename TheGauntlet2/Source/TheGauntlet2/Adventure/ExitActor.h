#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "ExitActor.generated.h"

class UMaterialInstanceDynamic;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API AExitActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    AExitActor();

    virtual void Interact_Implementation(AActor* Interactor) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
    TObjectPtr<UStaticMeshComponent> ExitMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Exit")
    TObjectPtr<USceneComponent> ArtifactPlacementPoint;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
    float ReturnToMenuDelay = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
    FLinearColor LockedColor = FLinearColor(0.6f, 0.0f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
    FLinearColor CompletedColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Exit")
    FName ColorParameterName = TEXT("Color");

private:
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

    bool bMissionCompleted = false;
    FTimerHandle ReturnTimerHandle;

    void ApplyExitColor(const FLinearColor& NewColor);
    void ReturnToMainMenu();
};
