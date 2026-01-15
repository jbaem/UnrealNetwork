#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "PSHUD.generated.h"

UCLASS()
class UNREALNETWORK_API APSHUD : public AHUD
{
	GENERATED_BODY()

public:
	class UPSMainWidget* GetMainWidget() const { return MainWidget; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPSMainWidget> MainWidgetClass;
	UPROPERTY()
	TObjectPtr<class UPSMainWidget> MainWidget;
};
