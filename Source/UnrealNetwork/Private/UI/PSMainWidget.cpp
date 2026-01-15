#include "UI/PSMainWidget.h"

#include "UI/DisplayValuesWidget.h"

void UPSMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreWidget->SetNameText(TEXT("Score: "));
}

void UPSMainWidget::UpdateScore(int32 NewScore)
{
	ScoreWidget->SetValueText(NewScore);
}