#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "RepPracHUD.generated.h"

UCLASS()
class UNREALNETWORK_API ARepPracHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	class URepPracMainWidget* GetWidget() const;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class URepPracMainWidget> WidgetClass;
	UPROPERTY()
	TObjectPtr<class URepPracMainWidget> MainWidget;
};
