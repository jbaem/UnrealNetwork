#include "GAS/ResourceAttributeSet.h"

#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"

UResourceAttributeSet::UResourceAttributeSet()
{
	InitMaxHealth(100.f);
	InitHealth(100.f);
}

void UResourceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UResourceAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UResourceAttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
}

void UResourceAttributeSet::PreAttributeChange(const FGameplayAttribute & Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	if(Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0);
	}

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0, GetMaxHealth());
	}
}

void UResourceAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)
{
	Super::PostGameplayEffectExecute(Data);

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		if(FMath::IsNearlyZero(GetHealth()))
		{
			// TODO: Handle death logic here
			UE_LOG(LogTemp, Warning, TEXT("Character has died."));
		}
	}
}

void UResourceAttributeSet::OnRep_Health(const FGameplayAttributeData & OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResourceAttributeSet, Health, OldMaxHealth);
}

void UResourceAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData & OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UResourceAttributeSet, MaxHealth, OldMaxHealth);
}
