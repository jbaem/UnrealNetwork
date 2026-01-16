#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "GSHUD.generated.h"

UCLASS()
class UNREALNETWORK_API AGSHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NT|UI")
	TSubclassOf<class UUserWidget> MainWidgetClass;
	UPROPERTY()
	TObjectPtr<class UUserWidget> MainWidget;
};
