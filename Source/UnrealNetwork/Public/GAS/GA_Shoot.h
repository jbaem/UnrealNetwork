#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"

#include "GA_Shoot.generated.h"

UCLASS()
class UNREALNETWORK_API UGA_Shoot : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Shoot();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	bool HitScan(FHitResult& OutHitResult, AActor* Avatar);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Shoot")
	TSubclassOf<UGameplayEffect> HitEffectClass;
	UPROPERTY(EditDefaultsOnly, Category = "NT|Ability|Shoot")
	float ShootRange = 1000.f;
	
};
