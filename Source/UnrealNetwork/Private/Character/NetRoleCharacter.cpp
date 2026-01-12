#include "Character/NetRoleCharacter.h"

ANetRoleCharacter::ANetRoleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANetRoleCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ANetRoleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintNetworkInfo();
}

void ANetRoleCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANetRoleCharacter::PrintNetworkInfo()
{
	const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
	const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());

	const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() ? GetNetConnection()->GetName() : TEXT("No NetConnection");

	const FString NetworkInfo = FString::Printf(
		TEXT("[LocalRole]   %s\n[RemoteRole]   %s\n[Owner]   %s\n[NetConnection]   %s"),
		*LocalRoleString,
		*RemoteRoleString,
		*OwnerString,
		*ConnectionString
	);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.f, true);
}