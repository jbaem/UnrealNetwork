#include "GAS/GA_Beam.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Character/GASCharacter.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


UGA_Beam::UGA_Beam()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerExecution;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGA_Beam::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	if (ActorInfo->AvatarActor.IsValid())
	{
		AvatarCharacter = Cast<AGASCharacter>(GetAvatarActorFromActorInfo());
	}

	if (AvatarCharacter)
	{
		AvatarCharacter->Server_RequestIgnoreMoveInput(true);
		if (BeamNiagaraSystem.IsValid())
		{
			AvatarCharacter->Multicast_StartBeam(BeamNiagaraSystem.Get(), BeamEndParamName);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		BeamTickTimer, 
		this,
		&UGA_Beam::BeamTick,
		TickRate, 
		true,
		0.f
	);
}

void UGA_Beam::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CleanupBeam();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Beam::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UGA_Beam::BeamTick()
{
	if (!AvatarCharacter)
	{
		return;
	}

	FHitResult HitResult;
	FVector EndPoint;
	PerformBeamTrace(HitResult, EndPoint);

	AvatarCharacter->Multicast_UpdateBeamEndPoint(BeamEndParamName, EndPoint);

	if(AvatarCharacter->HasAuthority())
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor != AvatarCharacter)
		{
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
			if (TargetASC)
			{
				ApplyDamageToTarget(TargetASC);
			}
		}
	}
}

void UGA_Beam::PerformBeamTrace(FHitResult& OutHit, FVector& OutEndPoint)
{
	if (AvatarCharacter)
	{
		FVector StartPoint = AvatarCharacter->GetActorLocation();
		OutEndPoint = StartPoint + AvatarCharacter->GetActorForwardVector() * BeamRange;
	
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(AvatarCharacter);
		GetWorld()->LineTraceSingleByChannel(
			OutHit, 
			StartPoint, OutEndPoint, 
			ECC_Camera, QueryParams
		);

		if (OutHit.bBlockingHit)
		{
			OutEndPoint = OutHit.ImpactPoint;
		}
	}
}

void UGA_Beam::ApplyDamageToTarget(UAbilitySystemComponent* TargetASC)
{
	if (TargetASC && DamageEffect)
	{
		UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
		if (SourceASC)
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
				DamageEffect,
				GetAbilityLevel(),
				ContextHandle
			);

			if(SpecHandle.IsValid())
			{
				SpecHandle.Data->SetSetByCallerMagnitude(
					FGameplayTag::RequestGameplayTag(FName("Data.Damage")),
					-DamagePerSecond * TickRate
				);
				
				SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
			}
		}
	}
}

void UGA_Beam::CleanupBeam()
{
	if (BeamTickTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(BeamTickTimer);
	}

	if (AvatarCharacter)
	{
		AvatarCharacter->Multicast_StopBeam();
		AvatarCharacter->Server_RequestIgnoreMoveInput(false);
	}
	AvatarCharacter = nullptr;
}
