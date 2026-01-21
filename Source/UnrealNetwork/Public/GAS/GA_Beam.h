#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GA_Beam.generated.h"

UCLASS()
class UNREALNETWORK_API UGA_Beam : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Beam();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

	virtual void InputReleased(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) override;

private:
	void BeamTick();
	void PerformBeamTrace(FHitResult& OutHit, FVector& OutEndPoint);
	void ApplyDamageToTarget(UAbilitySystemComponent* TargetASC);
	void CleanupBeam();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	float BeamRange = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	float DamagePerSecond = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	float TickRate = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	TWeakObjectPtr<class UNiagaraSystem> BeamNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	TWeakObjectPtr<class UNiagaraComponent> BeamNiagaraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Beam")
	FName BeamEndParamName = FName("BeamEnd");

private:
	FTimerHandle BeamTickTimer;

	class AGASCharacter* AvatarCharacter = nullptr;
};
