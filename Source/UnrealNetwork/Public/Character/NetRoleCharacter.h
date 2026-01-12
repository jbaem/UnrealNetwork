#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "NetRoleCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API ANetRoleCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()
	
public:
	ANetRoleCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void PrintNetworkInfo();

};
