#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TestGameState.generated.h"

UCLASS()
class UNREALNETWORK_API ATestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATestGameState();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "NT|Data")
	float GameEalpsedTime = 0.f;

};
