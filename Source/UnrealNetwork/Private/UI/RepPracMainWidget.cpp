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
	HealthDisplay->SetNameText(TEXT("Health: "));
}

void URepPracMainWidget::UpdateLevelDisplay(int32 InLevel)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UpdateLevelDisplay called with level: %d"), InLevel));
	LevelDisplay->SetValueText(InLevel);
}

void URepPracMainWidget::UpdateExpDisplay(float InExp)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UpdateExpDisplay called with exp: %f"), InExp));
	ExpDisplay->SetValueText(InExp);
}

void URepPracMainWidget::UpdateHealthDisplay(float InHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UpdateHealthDisplay called with health: %f"), InHealth));
	HealthDisplay->SetValueText(InHealth);
}