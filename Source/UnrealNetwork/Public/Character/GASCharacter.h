#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GASCharacter.generated.h"

DECLARE_DELEGATE(FOnAbilityPress)

struct FOnAttributeChangeData;

UCLASS()
class UNREALNETWORK_API AGASCharacter : public AUnrealNetworkCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	FOnAbilityPress OnAbilityPress;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	
	UFUNCTION(BlueprintCallable, Category = "NT|GAS")
	void TestActivateAbility();

	void OnInputAbilityPressed();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility();

private:
	void InitializeInputBind(AController* ControllerToBind);
	void ClearInputBind();
	void InitializeAbilitySystem();

public:
	class UResourceAttributeSet* GetResourceAttributeSet() const { return ResourceAS; }
	
	UFUNCTION(BlueprintCallable, Category = "NT|GAS|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "NT|GAS|Health")
	float GetMaxHealth() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NT|GAS")
	TObjectPtr<class UAbilitySystemComponent> ASC = nullptr;

	UPROPERTY()
	TObjectPtr<class UResourceAttributeSet> ResourceAS = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Ability")
	TSubclassOf<class UGameplayAbility> AbilityClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|UI")
	TObjectPtr<class UWidgetComponent> HealthWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Input")
	TObjectPtr<class UInputAction> Ability1InputAction = nullptr;

private:
	bool bAbilitySystemInitialized = false;
};
