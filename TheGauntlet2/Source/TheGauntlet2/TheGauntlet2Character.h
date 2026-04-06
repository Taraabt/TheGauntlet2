#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "TheGauntlet2Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USkeletalMeshComponent;
class UUserWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(Abstract)
class THEGAUNTLET2_API ATheGauntlet2Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

public:

	ATheGauntlet2Character();

	void HandlePause();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void DoInteract();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool HasArtifact() const { return bHasArtifact; }

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetHasArtifact(bool bNewHasArtifact);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetCarriedArtifact(AActor* NewArtifact);

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetCarriedArtifact() const { return CarriedArtifact.Get(); }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FName GetArtifactSocketName() const { return ArtifactSocketName; }

	UFUNCTION(BlueprintPure, Category = "Interaction")
	USkeletalMeshComponent* GetArtifactAttachMesh() const;
protected:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass;

	TObjectPtr<UUserWidget> PauseMenuInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionDistance = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	float InteractionTraceRadius = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Debug")
	bool bShowInteractionDebug = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Debug")
	FLinearColor InteractionDebugColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Debug")
	FLinearColor InteractionDebugHitColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Debug")
	float InteractionDebugDuration = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FName ArtifactSocketName = TEXT("ArtifactSocket");

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	bool bHasArtifact = false;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<AActor> CarriedArtifact;

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

public:

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
