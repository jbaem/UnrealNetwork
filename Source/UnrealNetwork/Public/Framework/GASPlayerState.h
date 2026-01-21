#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GAS/GASEnums.h"

#include "GASPlayerState.generated.h"

UCLASS()
class UNREALNETWORK_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGASPlayerState();

protected:
	virtual void BeginPlay() override;

public:
	void SetDefaultAbilitySystemData();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	class UResourceAttributeSet* GetResourceAttributeSet() const { return ResourceAS; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr< UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<class UResourceAttributeSet> ResourceAS = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Ability")
	TMap<EAbilityInputID, TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS|Effect")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;
};
