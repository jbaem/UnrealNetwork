#include "Framework/TestGameState.h"

#include "Net/UnrealNetwork.h"

ATestGameState::ATestGameState()
{

}

void ATestGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(HasAuthority())
	{
		GameEalpsedTime += DeltaSeconds;
	}
}

void ATestGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameState, GameEalpsedTime);
}
