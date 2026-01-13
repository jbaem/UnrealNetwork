#include "UI/DisplayValuesWidget.h"

#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UDisplayValuesWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDisplayValuesWidget::SetWidgetSize(float InWidth, float InHeight)
{
	RootSizeBox->SetWidthOverride(InWidth);
	RootSizeBox->SetHeightOverride(InHeight);
}

void UDisplayValuesWidget::SetNameText(const FString & InName)
{
	Name->SetText(FText::FromString(InName));
}

void UDisplayValuesWidget::SetValueText(const FString & InValue)
{
	Value->SetText(FText::FromString(InValue));
}

void UDisplayValuesWidget::SetValueText(int32 InValue)
{
	Value->SetText(FText::AsNumber(InValue));
}

void UDisplayValuesWidget::SetValueText(float InValue)
{
	Value->SetText(FText::AsNumber(InValue));
}

void UDisplayValuesWidget::SetColors(const FLinearColor& InColor)
{
	Name->SetColorAndOpacity(InColor);
	Value->SetColorAndOpacity(InColor);
}
