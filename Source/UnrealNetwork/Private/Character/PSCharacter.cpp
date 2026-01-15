#include "Character/PSCharacter.h"

#include "Framework/MyPlayerState.h"

APSCharacter::APSCharacter()
{}

void APSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APSCharacter::Server_AddScore_Implementation(int32 Point)
{
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->AddMyScore(Point);
	}
}

void APSCharacter::TestAddScore(int32 Point)
{
	if(IsLocallyControlled())
	{
		Server_AddScore(Point);
	}
}

