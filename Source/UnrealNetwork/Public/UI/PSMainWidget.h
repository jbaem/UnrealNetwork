#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PSMainWidget.generated.h"

UCLASS()
class UNREALNETWORK_API UPSMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateScore(int32 NewScore);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDisplayValuesWidget> ScoreWidget;
};
