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

#include "EnhancedInputComponent.h"
#include "InputAction.h"

#include "Framework/GASPlayerState.h"

AGASCharacter::AGASCharacter()
	: AUnrealNetworkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
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
		EnhancedInputComponent->BindAction(Ability1InputAction, ETriggerEvent::Started, this, &AGASCharacter::OnInputAbilityPressed);
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
			
			if (HealthWidget && HealthWidget->GetWidget())
			{
				UDisplayValuesWidget* HealthDisplayWidget = Cast<UDisplayValuesWidget>(HealthWidget->GetWidget());
				HealthDisplayWidget->SetNameText(FText::AsNumber(ResourceAS->GetHealth()));
				HealthDisplayWidget->SetValueText(FText::AsNumber(ResourceAS->GetMaxHealth()));
			}

			bAbilitySystemInitialized = true;
		}
	}

}

void AGASCharacter::OnHealthChanged(const FOnAttributeChangeData & Data)
{
	const float NewHealth = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("Health changed: %f"), NewHealth);

	if (HealthWidget && HealthWidget->GetWidget())
	{
		UDisplayValuesWidget* HealthDisplayWidget = Cast<UDisplayValuesWidget>(HealthWidget->GetWidget());
		HealthDisplayWidget->SetNameText(FText::AsNumber(ResourceAS->GetHealth()));
		HealthDisplayWidget->SetValueText(FText::AsNumber(ResourceAS->GetMaxHealth()));
	}
}

void AGASCharacter::OnInputAbilityPressed()
{
	OnAbilityPress.ExecuteIfBound();

	if (ASC)
	{
		UE_LOG(LogTemp, Log, TEXT("Input Success"));
		Server_ExecuteAbility();
	}
}

void AGASCharacter::Server_ExecuteAbility_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Call Server RPC"));
	ASC->AbilityLocalInputPressed(static_cast<int32>(EAbilityInputID::Shoot));
}

void AGASCharacter::TestActivateAbility()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("TestActivateAbility 시작"));
	if (ASC)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("TestActivateAbility : ASC 있음"));
		bool Result = ASC->TryActivateAbilityByClass(AbilityClass);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			FString::Printf(TEXT("TestActivateAbility : %s"),
				Result ? TEXT("성공") : TEXT("실패")));
	}
}

float AGASCharacter::GetHealth() const
{
	return ResourceAS ? ResourceAS->GetHealth() : 0.f;
}

float AGASCharacter::GetMaxHealth() const
{
	return ResourceAS ? ResourceAS->GetMaxHealth() : 0.f;
}