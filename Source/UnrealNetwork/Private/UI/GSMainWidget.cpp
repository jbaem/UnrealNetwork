#include "UI/GSMainWidget.h"

#include "Components/TextBlock.h"
#include "Framework/TestGameState.h"
#include "Kismet/GameplayStatics.h"

void UGSMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameOverText->SetVisibility(ESlateVisibility::Hidden);
}

void UGSMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateTimeText();
}

void UGSMainWidget::UpdateTimeText()
{
	if (!CachedGameState.IsValid())
	{
		ATestGameState* GameState = Cast<ATestGameState>(UGameplayStatics::GetGameState(GetWorld()));
		SetCachedGameState(GameState);
	}

	if(CachedGameState.IsValid())
	{
		UpdateAllTimes();
	}
}

void UGSMainWidget::ShowGameOverText()
{
	GameOverText->SetVisibility(ESlateVisibility::Visible);
}

void UGSMainWidget::SetCachedGameState(ATestGameState* InGameState)
{
	if (InGameState)
	{
		CachedGameState = InGameState;
	}
}

void UGSMainWidget::UpdateAllTimes()
{
	int32 ElapsedTimeInt = FMath::FloorToInt(CachedGameState->GetElapsedTime()); // Use Modern Unreal function
	SetTimeText(ElapsedTimeText, ElapsedTimeInt, TEXT("[Time]"));

	int32 RemainTimeInt = FMath::CeilToInt(CachedGameState->GetRemainTime());
	SetTimeText(RemainTimeText, RemainTimeInt, TEXT("[Remain]"));
}

void UGSMainWidget::SetTimeText(UTextBlock* InTextBlock, int32 InTime, const FString& Prefix)
{
	if (InTextBlock)
	{
		int32 Minutes = InTime / 60;
		int32 Seconds = InTime % 60;
		InTextBlock->SetText(FText::FromString(FString::Printf(
			TEXT("%s %02d : %02d"), 
			*Prefix, Minutes, Seconds
		)));
	}
}
