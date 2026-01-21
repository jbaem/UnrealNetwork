#include "Character/GASCharacter.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "GAS/ResourceAttributeSet.h"
#include "GAS/GASEnums.h"

#include "UI/DisplayValuesWidget.h"
#include "UI/BilboardWidgetComponent.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "Framework/GASPlayerState.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

AGASCharacter::AGASCharacter()
	: AUnrealNetworkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HealthWidget = CreateDefaultSubobject<UBilboardWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(RootComponent);

}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AGASCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ClearInputBind();
	Super::EndPlay(EndPlayReason);
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(Ability1InputAction, ETriggerEvent::Started, this, &AGASCharacter::OnInputAbility1Pressed);
		EnhancedInputComponent->BindAction(Ability2InputAction, ETriggerEvent::Started, this, &AGASCharacter::OnInputAbility2Pressed);
		EnhancedInputComponent->BindAction(Ability2InputAction, ETriggerEvent::Completed, this, &AGASCharacter::OnInputAbility2Released);
	}
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitializeInputBind(NewController);
	InitializeAbilitySystem(); // for server side
}

void AGASCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void AGASCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystem(); // for client side
}

void AGASCharacter::InitializeInputBind(AController* ControllerToBind)
{
}

void AGASCharacter::ClearInputBind()
{	
	if (OnAbility1Press.IsBound())
	{
		OnAbility1Press.Unbind();
	}
	if (OnAbility2Press.IsBound())
	{
		OnAbility2Press.Unbind();
	}
	if(OnAbility2Release.IsBound())
	{
		OnAbility2Release.Unbind();
	}
}

void AGASCharacter::InitializeAbilitySystem()
{
	if (bAbilitySystemInitialized)
	{
		return;
	}

	if (AGASPlayerState* PS = GetPlayerState<AGASPlayerState>())
	{
		ASC = PS->GetAbilitySystemComponent();
		ResourceAS = PS->GetResourceAttributeSet();
		if (ASC && ResourceAS)
		{
			ASC->InitAbilityActorInfo(this, this);

			FOnGameplayAttributeValueChange& OnHealthChangeDel = ASC->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetHealthAttribute());
			OnHealthChangeDel.AddUObject(this, &AGASCharacter::OnHealthChanged);
			
			UpdateHealthWidget();

			bAbilitySystemInitialized = true;

			PS->SetDefaultAbilitySystemData();
		}
	}

}

void AGASCharacter::OnHealthChanged(const FOnAttributeChangeData & Data)
{
	const float NewHealth = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("Health changed: %f"), NewHealth);

	UpdateHealthWidget();
}

void AGASCharacter::Multicast_StopBeam_Implementation()
{
	if(BeamNiagaraComponent)
	{
		BeamNiagaraComponent->DestroyComponent();
		BeamNiagaraComponent = nullptr;
	}
}

void AGASCharacter::Multicast_UpdateBeamEndPoint_Implementation(FName BeamEndParam, const FVector& EndPoint)
{
	if (BeamNiagaraComponent)
	{
		BeamNiagaraComponent->SetVariablePosition(BeamEndParam, EndPoint);
	}
}

void AGASCharacter::Multicast_StartBeam_Implementation(UNiagaraSystem* BeamSystem, FName BeamEndParam)
{
	if (BeamSystem)
	{
		if (BeamNiagaraComponent)
		{
			BeamNiagaraComponent->DestroyComponent();
			BeamNiagaraComponent = nullptr;
		}

		BeamNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BeamSystem,
			GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if(BeamNiagaraComponent)
		{
			// Set the beam end point to initial location (= character location)
			BeamNiagaraComponent->SetVariablePosition(BeamEndParam, GetActorLocation());
		}
	}
}

void AGASCharacter::UpdateHealthWidget()
{
	if (HealthWidget && HealthWidget->GetWidget())
	{
		UDisplayValuesWidget* HealthDisplayWidget = Cast<UDisplayValuesWidget>(HealthWidget->GetWidget());
		HealthDisplayWidget->SetNameText(FText::AsNumber(ResourceAS->GetHealth()));
		HealthDisplayWidget->SetValueText(FText::AsNumber(ResourceAS->GetMaxHealth()));
	}
}

void AGASCharacter::OnInputAbility1Pressed()
{
	OnAbility1Press.ExecuteIfBound();
	if (ASC)
	{
		Server_ExecuteAbility1();
	}
}

void AGASCharacter::OnInputAbility2Pressed()
{
	OnAbility2Press.ExecuteIfBound();
	if (ASC)
	{
		Server_ExecuteAbility2();
	}
}

void AGASCharacter::OnInputAbility2Released()
{
	OnAbility2Release.ExecuteIfBound();
	if (ASC)
	{
		Server_EndAbility2();
	}
}

void AGASCharacter::Server_ExecuteAbility1_Implementation()
{
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Shoot));
}

void AGASCharacter::Server_ExecuteAbility2_Implementation()
{
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Beam));
}

void AGASCharacter::Server_EndAbility2_Implementation()
{
	ASC->AbilityLocalInputReleased(static_cast<int32>(EAbilityInputID::Beam));
}

float AGASCharacter::GetHealth() const
{
	return ResourceAS ? ResourceAS->GetHealth() : 0.f;
}

float AGASCharacter::GetMaxHealth() const
{
	return ResourceAS ? ResourceAS->GetMaxHealth() : 0.f;
}

void AGASCharacter::Server_RequestIgnoreMoveInput_Implementation(bool bIgnore)
{
	if(APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->SetIgnoreMoveInput(bIgnore);
	}
}
