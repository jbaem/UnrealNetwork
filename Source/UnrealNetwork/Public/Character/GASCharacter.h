#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GASCharacter.generated.h"

DECLARE_DELEGATE(FOnAbilityInput)

struct FOnAttributeChangeData;

UCLASS()
class UNREALNETWORK_API AGASCharacter : public AUnrealNetworkCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGASCharacter();

	FOnAbilityInput OnAbility1Press;
	FOnAbilityInput OnAbility2Press;
	FOnAbilityInput OnAbility2Release;

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
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartBeam(class UNiagaraSystem* BeamSystem, FName BeamEndParam);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_UpdateBeamEndPoint(FName BeamEndParam, const FVector& EndPoint);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopBeam();

	UFUNCTION(Server, Reliable)
	void Server_RequestIgnoreMoveInput(bool bIgnore);

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility1();

	UFUNCTION(Server, Reliable)
	void Server_ExecuteAbility2();

	UFUNCTION(Server, Reliable)
	void Server_EndAbility2();

protected:
	void OnInputAbility1Pressed();
	void OnInputAbility2Pressed();
	void OnInputAbility2Released();

private:
	void InitializeInputBind(AController* ControllerToBind);
	void ClearInputBind();
	void InitializeAbilitySystem();
	void UpdateHealthWidget();

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
	TObjectPtr<class UBilboardWidgetComponent> HealthWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Input")
	TObjectPtr<class UInputAction> Ability1InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Input")
	TObjectPtr<class UInputAction> Ability2InputAction = nullptr;

private:
	bool bAbilitySystemInitialized = false;

	UPROPERTY()
	class UNiagaraComponent* BeamNiagaraComponent = nullptr;
};
