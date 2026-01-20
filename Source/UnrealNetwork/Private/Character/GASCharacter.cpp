#include "Character/GASCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectTypes.h"

#include "GAS/ResourceAttributeSet.h"

AGASCharacter::AGASCharacter()
	: AUnrealNetworkCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

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

	if (ASC)
	{
		ASC->InitAbilityActorInfo(this, this);
		
		FOnGameplayAttributeValueChange& OnHealthChangeDel = ASC->GetGameplayAttributeValueChangeDelegate(UResourceAttributeSet::GetHealthAttribute());
		OnHealthChangeDel.AddUObject(this, &AGASCharacter::OnHealthChanged);
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
}
