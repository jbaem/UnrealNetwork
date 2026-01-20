#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GAS/GASMacros.h"

#include "ResourceAttributeSet.generated.h"

UCLASS()
class UNREALNETWORK_API UResourceAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UResourceAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

public:
	UPROPERTY(BlueprintReadOnly, Category = "NT|Attributes|Health", ReplicatedUsing = OnRep_Health);
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UResourceAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, Category = "NT|Attributes|Health", ReplicatedUsing = OnRep_MaxHealth);
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UResourceAttributeSet, MaxHealth);

};
