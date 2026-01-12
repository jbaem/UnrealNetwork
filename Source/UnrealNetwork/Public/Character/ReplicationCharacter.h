#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"
#include "ReplicationCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API AReplicationCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	AReplicationCharacter();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRepNotify_Level();

protected:
	UPROPERTY(ReplicatedUsing = OnRepNotify_Level)		// Replicated with notification function
	int32 Level = 1;

	UPROPERTY(Replicated)		// Replicated but no notification function
	float Health = 100.0f;

	UPROPERTY(Replicated)
	float Exp = 0.0f;
};
