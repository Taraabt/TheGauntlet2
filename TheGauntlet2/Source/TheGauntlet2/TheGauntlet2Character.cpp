#include "TheGauntlet2Character.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Engine/EngineTypes.h"

#include "Core/MyGameInstance.h"
#include "Interaction/Interactable.h"

ATheGauntlet2Character::ATheGauntlet2Character()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);

	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;
}

void ATheGauntlet2Character::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void ATheGauntlet2Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetActorLocation().Z <= RespawnZThreshold)
	{
		RespawnAtPlayerStart();
	}
}

void ATheGauntlet2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATheGauntlet2Character::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATheGauntlet2Character::Look);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATheGauntlet2Character::Look);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ATheGauntlet2Character::HandlePause);

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ATheGauntlet2Character::DoInteract);
		}
	}
}

void ATheGauntlet2Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATheGauntlet2Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATheGauntlet2Character::DoMove(float Right, float Forward)
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDir, Forward);
		AddMovementInput(RightDir, Right);
	}
}

void ATheGauntlet2Character::DoLook(float Yaw, float Pitch)
{
	AddControllerYawInput(Yaw);
	AddControllerPitchInput(Pitch);
}

void ATheGauntlet2Character::DoJumpStart()
{
	Jump();
}

void ATheGauntlet2Character::DoJumpEnd()
{
	StopJumping();
}
void ATheGauntlet2Character::HandlePause()
{
	if (UMyGameInstance* GI = GetGameInstance<UMyGameInstance>())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());

		if (!PC) return;

		if (GI->CurrentState == EGameState::Paused)
		{
			GI->SetPaused(false);

			if (PauseMenuInstance)
			{
				PauseMenuInstance->RemoveFromParent();
			}

			FInputModeGameOnly InputMode;
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = false;
		}
		else
		{
			GI->SetPaused(true);

			if (PauseMenuClass)
			{
				PauseMenuInstance = CreateWidget<UUserWidget>(PC, PauseMenuClass);
				if (PauseMenuInstance)
				{
					PauseMenuInstance->AddToViewport();
				}
			}

			FInputModeUIOnly InputMode;
			if (PauseMenuInstance)
			{
				InputMode.SetWidgetToFocus(PauseMenuInstance->TakeWidget());
			}
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
	}
}

void ATheGauntlet2Character::DoInteract()
{
	if (!GetWorld())
	{
		return;
	}

	const FVector TraceStart = GetActorLocation() + FVector(0.0f, 0.0f, InteractionHeightOffset);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * InteractionDistance);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerInteractTrace), false, this);
	FHitResult HitResult;
	const bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(InteractionTraceRadius),
		QueryParams
	);

	if (!bHit || !HitResult.GetActor())
	{
		return;
	}

    if (AActor* InteractableActor = HitResult.GetActor();
        InteractableActor && InteractableActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
    {
        IInteractable::Execute_Interact(InteractableActor, this);
    }
}

void ATheGauntlet2Character::SetHasArtifact(bool bNewHasArtifact)
{
	bHasArtifact = bNewHasArtifact;
}

void ATheGauntlet2Character::SetCarriedArtifact(AActor* NewArtifact)
{
	CarriedArtifact = NewArtifact;
}

USkeletalMeshComponent* ATheGauntlet2Character::GetArtifactAttachMesh() const
{
	return GetMesh();
}

void ATheGauntlet2Character::FellOutOfWorld(const UDamageType& dmgType)
{
	RespawnAtPlayerStart();
}

void ATheGauntlet2Character::RespawnAtPlayerStart()
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	if (PlayerStarts.Num() > 0)
	{
		const FTransform RespawnTransform = PlayerStarts[0]->GetActorTransform();
		SetActorLocationAndRotation(
			RespawnTransform.GetLocation(),
			RespawnTransform.GetRotation()
		);

		GetCharacterMovement()->StopMovementImmediately();
		return;
	}
}
