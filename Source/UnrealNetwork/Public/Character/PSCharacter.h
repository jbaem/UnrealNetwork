#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"
#include "PSCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API APSCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	APSCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_AddScore(int32 Point);

public:
	UFUNCTION(BlueprintCallable, Category = "NT|Test")
	void TestAddScore(int32 Point);

};
