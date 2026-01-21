#include "Framework/GASPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/ResourceAttributeSet.h"

AGASPlayerState::AGASPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	// Minimal : Only attribute and cue replication to owner and other clients. (ex. NPC)
	// Mixed : Minimal + all replication to owner (ex. Player)
	// Full : All replication to everyone (Debug, Spectator)

	ResourceAS = CreateDefaultSubobject<UResourceAttributeSet>(TEXT("ResourceAS"));
}

void AGASPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetDefaultAbilitySystemData();
	}
}

void AGASPlayerState::SetDefaultAbilitySystemData()
{
	if (HasAuthority() && ASC)
	{
		for(const auto& AbilityPair : DefaultAbilities)
		{
			if (AbilityPair.Value)
			{
				ASC->GiveAbility(FGameplayAbilitySpec(AbilityPair.Value, 1, static_cast<int32>(AbilityPair.Key), this));
			}
		}

		for (auto& EffectClass : DefaultEffects)
		{
			if (EffectClass)
			{
				FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
				ContextHandle.AddSourceObject(this);
				
				FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1, ContextHandle);
				if(SpecHandle.IsValid())
				{
					ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
				}
			}
		}
	}
}
