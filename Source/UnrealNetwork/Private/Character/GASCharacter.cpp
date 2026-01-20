#include "Character/GASCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

AGASCharacter::AGASCharacter()
	: AUnrealNetworkCharacter()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}
