#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GASCharacter.generated.h"

struct FOnAttributeChangeData;

UCLASS()
class UNREALNETWORK_API AGASCharacter : public AUnrealNetworkCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NT|GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<class UResourceAttributeSet> ResourceAttributeSet = nullptr;
};
