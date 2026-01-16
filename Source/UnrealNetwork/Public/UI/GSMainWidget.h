#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "GSMainWidget.generated.h"

UCLASS()
class UNREALNETWORK_API UGSMainWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "NT|UI")
	void UpdateTimeText();

public:
	UFUNCTION(BlueprintCallable)
	void ShowGameOverText();

	void SetCachedGameState(class ATestGameState* InGameState);

private:
	void UpdateAllTimes();
	void SetTimeText(class UTextBlock* InTextBlock, int32 InTime, const FString& Prefix);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> ElapsedTimeText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> RemainTimeText;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UTextBlock> GameOverText;

	UPROPERTY()
	TWeakObjectPtr<class ATestGameState> CachedGameState;
};
