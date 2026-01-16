#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "TestGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOverDelegate);

UCLASS()
class UNREALNETWORK_API ATestGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATestGameState();

	FOnGameOverDelegate OnGameOverDel;

	bool IsGameOver() const { return bIsGameOver; }

	float GetElapsedTime() const { return GameEalpsedTime; }
	float GetRemainTime() const { return GameRemainTime; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_IsGameOver();

private:
	void OnGameOver();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Data")
	float GameOverTimeLimit = 50.f; // TODO: Move to GameMode
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "NT|Data")
	float GameEalpsedTime = 0.f;
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "NT|Data")
	float GameRemainTime = 50.f;

	UPROPERTY(ReplicatedUsing = OnRep_IsGameOver, BlueprintReadOnly, Category = "NT|Data")
	bool bIsGameOver = false;
};
