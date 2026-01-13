#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "RepPracMainWidget.generated.h"

UCLASS()
class UNREALNETWORK_API URepPracMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void InitializeDisplays();

	UFUNCTION()
	void UpdateLevelDisplay(int32 InLevel);
	UFUNCTION()
	void UpdateExpDisplay(float InExp);

public:
	UPROPERTY(meta = (BindWidget))
	class UDisplayValuesWidget* LevelDisplay = nullptr;
	UPROPERTY(meta = (BindWidget))
	class UDisplayValuesWidget* ExpDisplay = nullptr;
};
