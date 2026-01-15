#include "Framework/MyPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "UI/PSHUD.h"
#include "UI/PSMainWidget.h"

#include "Character/PSCharacter.h"

AMyPlayerState::AMyPlayerState()
{}

void AMyPlayerState::AddMyScore(int32 InScore)
{
	if (HasAuthority())
	{
		MyScore += InScore;
		if (APSCharacter* MyChar = Cast<APSCharacter>(GetPawn()))
		{
			MyChar->UpdateScore(MyScore);
		}

		OnMyScoreChanged.Broadcast(MyScore);
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
	APlayerController* PC = GetPlayerController();
	if (PC)
	{
		if (PC->IsLocalController())
		{
			AHUD* MyHUD = GetWorld()->GetFirstPlayerController()->GetHUD();
			APSHUD* PSHud = Cast<APSHUD>(MyHUD);

			PSHud->GetMainWidget()->UpdateScore(MyScore);
		}

	}
}
