#include "UI/GSHUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GSMainWidget.h"
#include "Framework/TestGameState.h"

void AGSHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}

	ATestGameState* GameState = Cast<ATestGameState>(UGameplayStatics::GetGameState(GetWorld()));
	UGSMainWidget* GSMainWidget = Cast<UGSMainWidget>(MainWidget);
	if (GameState && GSMainWidget)
	{
		GSMainWidget->SetCachedGameState(GameState);
		GameState->OnGameOverDel.AddDynamic(GSMainWidget, &UGSMainWidget::ShowGameOverText);
	}
}
