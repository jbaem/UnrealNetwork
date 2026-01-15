#include "UI/PSMainWidget.h"

#include "UI/DisplayValuesWidget.h"
#include "Components/EditableText.h"

#include "Framework/MyPlayerState.h"

void UPSMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ScoreWidget->SetNameText(TEXT("Score: "));
	ScoreWidget->SetValueText(0);

	PlayerName->OnTextCommitted.AddDynamic(this, &UPSMainWidget::NameCommitted);
}

void UPSMainWidget::UpdateScore(int32 NewScore)
{
	ScoreWidget->SetValueText(NewScore);
}

void UPSMainWidget::NameCommitted(const FText& InText, ETextCommit::Type InCommit)
{
	AMyPlayerState* PS = GetOwningPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->SetMyName(InText.ToString());
	}
}
