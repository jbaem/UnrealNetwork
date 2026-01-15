#include "Framework/MyPlayerState.h"

#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{}

void AMyPlayerState::AddMyScore(int32 InScore)
{
	if (HasAuthority())
	{
		MyScore += InScore;
		// Server does not have replication, call manipulating
		OnRep_MyScore();
	}

}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, MyScore); // Replicate all clients.
}

void AMyPlayerState::OnRep_MyScore()
{
	// TODO: Update UI
	UE_LOG(LogTemp, Log, TEXT("Score : %d"), MyScore);
}
