#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "DisplayValuesWidget.generated.h"

UCLASS()
class UNREALNETWORK_API UDisplayValuesWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetWidgetSize(float InWidth, float InHeight);
	void SetNameText(const FString& InName);
	void SetNameText(FText InName);
	void SetValueText(const FString& InValue);
	void SetValueText(int32 InValue);
	void SetValueText(FText InValue);
	void SetValueText(float InValue);
	void SetColors(const FLinearColor& InColor);

public: 
	UPROPERTY(meta = (BindWidget))
	class USizeBox* RootSizeBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Value = nullptr;
};
