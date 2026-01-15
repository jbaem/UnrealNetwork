#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Types/SlateEnums.h"

#include "PSMainWidget.generated.h"

UCLASS()
class UNREALNETWORK_API UPSMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateScore(int32 NewScore);

	UFUNCTION()
	void NameCommitted(const FText& InText, ETextCommit::Type InCommit);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDisplayValuesWidget> ScoreWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> PlayerName;
};
