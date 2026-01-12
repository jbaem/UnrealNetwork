#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "ConnectionCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API AConnectionCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	AConnectionCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	void PrintNetworkInfo();

};
