#include "Framework/TestGameState.h"

#include "Net/UnrealNetwork.h"

ATestGameState::ATestGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATestGameState::BeginPlay()
{
	Super::BeginPlay();
	
	GameRemainTime = GameOverTimeLimit;
	
}

void ATestGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HasAuthority() && !bIsGameOver)
	{
		GameEalpsedTime += DeltaSeconds;
		GameRemainTime -= DeltaSeconds;

		if(GameRemainTime < 0.f)
		{
			GameRemainTime = 0.f;
			OnGameOver();
		}
	}
}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, GameEalpsedTime);
	DOREPLIFETIME(ATestGameState, GameRemainTime);
	DOREPLIFETIME(ATestGameState, bIsGameOver);
}

void ATestGameState::OnRep_IsGameOver()
{
	if (bIsGameOver)
	{
		OnGameOverDel.Broadcast();
	}
}

void ATestGameState::OnGameOver()
{
	if(HasAuthority())
	{
		bIsGameOver = true;
		OnRep_IsGameOver();
	}
}
