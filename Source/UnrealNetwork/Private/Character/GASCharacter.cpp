#include "Character/GASCharacter.h"

#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"
#include "GAS/ResourceAttributeSet.h"

#include "UI/DisplayValuesWidget.h"

AGASCharacter::AGASCharacter()
	: AUnrealNetworkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidget->SetupAttachment(RootComponent);

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// Minimal : Only attribute and cue replication to owner and other clients. (ex. NPC)
	// Mixed : Minimal + all replication to owner (ex. Player)
	// Full : All replication to everyone (Debug, Spectator)

	ResourceAttributeSet = CreateDefaultSubobject<UResourceAttributeSet>(TEXT("ResourceAttributeSet"));

}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	if (ASC && AbilityClass)
	{
		ASC->InitAbilityActorInfo(this, this);
		
		FOnGameplayAttributeValueChange& OnHealthChangeDel = ASC->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetHealthAttribute());
		OnHealthChangeDel.AddUObject(this, &AGASCharacter::OnHealthChanged);
	
		if (HealthWidget && HealthWidget->GetWidget())
		{
			UDisplayValuesWidget* HealthDisplayWidget = Cast<UDisplayValuesWidget>(HealthWidget->GetWidget());
			HealthDisplayWidget->SetNameText(FText::AsNumber(ResourceAttributeSet->GetHealth()));
			HealthDisplayWidget->SetValueText(FText::AsNumber(ResourceAttributeSet->GetMaxHealth()));
		}

		if (HasAuthority())
		{
			ASC->GiveAbility(FGameplayAbilitySpec(AbilityClass, 1, -1, this));
		}
	}
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGASCharacter::OnHealthChanged(const FOnAttributeChangeData & Data)
{
	const float NewHealth = Data.NewValue;
	UE_LOG(LogTemp, Log, TEXT("Health changed: %f"), NewHealth);

	if (HealthWidget && HealthWidget->GetWidget())
	{
		UDisplayValuesWidget* HealthDisplayWidget = Cast<UDisplayValuesWidget>(HealthWidget->GetWidget());
		HealthDisplayWidget->SetNameText(FText::AsNumber(ResourceAttributeSet->GetHealth()));
		HealthDisplayWidget->SetValueText(FText::AsNumber(ResourceAttributeSet->GetMaxHealth()));
	}
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