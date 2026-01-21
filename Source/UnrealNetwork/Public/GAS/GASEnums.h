#pragma once

#include "CoreMinimal.h"

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"

UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	Shoot = 0	UMETA(DisplayName = "Shoot"),
};