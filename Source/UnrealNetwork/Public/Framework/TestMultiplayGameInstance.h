#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "TestMultiplayGameInstance.generated.h"

UCLASS()
class UNREALNETWORK_API UTestMultiplayGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTestMultiplayGameInstance();

	virtual void Init() override;

	void CreateServer();
	void JoinServer(FString IPAddress);
	void DisconnectServer();

	inline const FString& GetServerIP() const { return ServerIP; }

private:
	UFUNCTION()
	void HandleNetworkFailued(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	UFUNCTION()
	void HandleTravelFailure(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|Test")
	FString ServerIP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|Test")
	TSoftObjectPtr<UWorld> MainLevelAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|Test")
	TSoftObjectPtr<UWorld> ServerLevelAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|Test")
	int32 MaxPlayers = 2;

};
