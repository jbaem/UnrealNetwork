#include "UI/RepPracMainWidget.h"

#include "UI/DisplayValuesWidget.h"

void URepPracMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeDisplays();
}

void URepPracMainWidget::InitializeDisplays()
{
	LevelDisplay->SetNameText(TEXT("Level: "));
	ExpDisplay->SetNameText(TEXT("Exp: "));
}

void URepPracMainWidget::UpdateLevelDisplay(int32 InLevel)
{
	LevelDisplay->SetValueText(InLevel);
}

void URepPracMainWidget::UpdateExpDisplay(float InExp)
{
	ExpDisplay->SetValueText(InExp);
}
