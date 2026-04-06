#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "LeverInteractableActor.generated.h"

class ADefenseTurretActor;
class AOrbitingPlatformSystem;
class ASimpleDoorActor;
class UMaterialInstanceDynamic;
class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API ALeverInteractableActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ALeverInteractableActor();

    virtual void Interact_Implementation(AActor* Interactor) override;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lever")
    TObjectPtr<UStaticMeshComponent> LeverMesh;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Lever Targets")
    TObjectPtr<AOrbitingPlatformSystem> ControlledPlatforms;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Lever Targets")
    TObjectPtr<ASimpleDoorActor> ControlledDoor;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Lever Targets")
    TObjectPtr<ADefenseTurretActor> ControlledTurret;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lever")
    float TurretDisabledDuration = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lever")
    FLinearColor InactiveColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lever")
    FLinearColor ActiveColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Lever")
    FName ColorParameterName = TEXT("Color");

private:
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

    bool bIsActivated = false;
    void ApplyLeverColor(const FLinearColor& NewColor);
};
