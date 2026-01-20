#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "GASCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API AGASCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	AGASCharacter();

protected:
	virtual void BeginPlay() override;
	
};
