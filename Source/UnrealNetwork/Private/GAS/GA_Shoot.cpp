#include "GAS/GA_Shoot.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_Shoot::UGA_Shoot()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_Shoot::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	if (!HitEffectClass)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	AActor* Avatar = ActorInfo->AvatarActor.Get();
	FHitResult HitResult;
	if (HitScan(HitResult, Avatar))
	{
		DrawDebugLine(GetWorld(), Avatar->GetActorLocation(), HitResult.ImpactPoint, FColor::Yellow, false, 0.1f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, 0.1f);

		AActor* Target = HitResult.GetActor();
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

		if (Target && HitEffectClass)
		{
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddHitResult(HitResult);
			ContextHandle.AddInstigator(Avatar->GetInstigator(), Avatar);

			FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(HitEffectClass, 1, ContextHandle);
			if(SpecHandle.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Avatar->GetActorLocation(), Avatar->GetActorLocation() + Avatar->GetActorForwardVector() * ShootRange, FColor::Green, false, 0.1f, 0, 1.0f);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

bool UGA_Shoot::HitScan(FHitResult& OutHitResult, AActor* Avatar)
{
	FVector Start = Avatar->GetActorLocation();
	FVector End = Start + Avatar->GetActorForwardVector() * ShootRange;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Avatar);

	return GetWorld()->LineTraceSingleByObjectType(
		OutHitResult,
		Start,
		End,
		ECC_Pawn,
		QueryParams
	);
}
