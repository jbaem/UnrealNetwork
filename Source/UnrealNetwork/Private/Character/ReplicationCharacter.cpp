#include "Character/ReplicationCharacter.h"

#include "Net/UnrealNetwork.h"

AReplicationCharacter::AReplicationCharacter()
{}

void AReplicationCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AReplicationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		Level++;
		Health += 0.1f;
		Exp += 0.05f;
	}

	const FString Str = FString::Printf(TEXT("Level: %d, Health: %.2f, Exp: %.2f"), Level, Health, Exp);
	DrawDebugString(GetWorld(), GetActorLocation(), Str, nullptr, FColor::White, 0.f, true);
}

void AReplicationCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AReplicationCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AReplicationCharacter, Level, COND_OwnerOnly);			// Level replicated only to owner
	DOREPLIFETIME(AReplicationCharacter, Health);									// Health must be replicated
	DOREPLIFETIME_CONDITION(AReplicationCharacter, Exp, COND_SimulatedOnly);		// Exp replicated only to simulated proxies (non-owners)

}

void AReplicationCharacter::OnRepNotify_Level()
{
	const FString Str = FString::Printf(TEXT("OnRepNotify_Level called: Level = %d"), Level);
	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, Str);
}
