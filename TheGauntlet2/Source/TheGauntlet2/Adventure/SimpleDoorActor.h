#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimpleDoorActor.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class THEGAUNTLET2_API ASimpleDoorActor : public AActor
{
    GENERATED_BODY()

public:
    ASimpleDoorActor();

    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = "Door")
    void OpenDoor();

    UFUNCTION(BlueprintCallable, Category = "Door")
    void CloseDoor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<USceneComponent> SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    FVector OpenOffset = FVector(0.0f, 0.0f, 300.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door", meta = (ClampMin = "0.0"))
    float OpenDuration = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
    bool bStartsOpen = false;

private:
    bool bIsOpen = false;
    FVector ClosedRelativeLocation = FVector::ZeroVector;
    FVector OpenRelativeLocation = FVector::ZeroVector;
    float CurrentMoveAlpha = 0.0f;
};
