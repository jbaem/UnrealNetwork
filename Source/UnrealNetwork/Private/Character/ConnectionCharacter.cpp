#include "Character/ConnectionCharacter.h"

AConnectionCharacter::AConnectionCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AConnectionCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConnectionCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PrintNetworkInfo();
}

void AConnectionCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AConnectionCharacter::PrintNetworkInfo()
{
	const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() ? GetNetConnection()->GetName() : TEXT("No NetConnection");

	const FString NetworkInfo = FString::Printf(
		TEXT("[Owner]   %s\n[NetConnection]   %s"), 
		*OwnerString, 
		*ConnectionString
	);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.f, true);
}
