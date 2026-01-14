#include "Character/RepPracCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Net/UnrealNetwork.h"

#include "GameFramework/PlayerController.h"
#include "Components/WidgetComponent.h"
#include "UI/DisplayValuesWidget.h"

ARepPracCharacter::ARepPracCharacter()
{
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void ARepPracCharacter::BeginPlay()
{
	Super::BeginPlay();

	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	if (UDisplayValuesWidget* Widget = Cast<UDisplayValuesWidget>(WidgetComponent->GetWidget()))
	{
		Widget->SetWidgetSize(300.f, 50.f);
		Widget->SetColors(FLinearColor::Red);
		Widget->SetNameText(TEXT("Health: "));
		Widget->SetValueText(Health);
		OnRepNotify_Health();
	}
}

void ARepPracCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (WidgetComponent)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if(PC && PC->PlayerCameraManager)
		{
			// way 1: look at camera
			//FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			//FVector ToCamera = CameraLocation - WidgetComponent->GetComponentLocation();
			//FRotator LookAtRotation = FRotationMatrix::MakeFromX(ToCamera).Rotator();
			
			// way 2: face camera forward vector
			FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(-CameraForward).Rotator();
			WidgetComponent->SetWorldRotation(LookAtRotation);
		}
	}
}

void ARepPracCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(Task1Action, ETriggerEvent::Started, this, &ARepPracCharacter::OnTask1);
		EnhancedInputComponent->BindAction(Task2Action, ETriggerEvent::Started, this, &ARepPracCharacter::OnTask2);
		EnhancedInputComponent->BindAction(Task3Action, ETriggerEvent::Started, this, &ARepPracCharacter::OnTask3);
	}
}

void ARepPracCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ARepPracCharacter, Level, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ARepPracCharacter, Exp, COND_OwnerOnly);
	
	DOREPLIFETIME(ARepPracCharacter, Health);
}

void ARepPracCharacter::Server_OnTask1_Implementation()
{
	Level += 1;
	OnRepNotify_Level();
}

void ARepPracCharacter::Server_OnTask2_Implementation()
{
	Exp += 5.f;
	OnRepNotify_Exp();
}

void ARepPracCharacter::Server_OnTask3_Implementation()
{
	Health += 10.f;
	OnRepNotify_Health();
}

void ARepPracCharacter::OnRepNotify_Level()
{
	OnLevelChange.Broadcast(Level);
}

void ARepPracCharacter::OnRepNotify_Exp()
{
	OnExpChange.Broadcast(Exp);
}

void ARepPracCharacter::OnRepNotify_Health()
{
	if (UDisplayValuesWidget* Widget = Cast<UDisplayValuesWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		Widget->SetValueText(Health);
	}
	OnHealthChange.Broadcast(Health);
}
