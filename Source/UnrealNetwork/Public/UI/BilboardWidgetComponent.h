#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"

#include "BilboardWidgetComponent.generated.h"

UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UNREALNETWORK_API UBilboardWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
