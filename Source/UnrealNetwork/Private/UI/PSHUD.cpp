#include "UI/PSHUD.h"

#include "UI/PSMainWidget.h"

void APSHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UPSMainWidget>(GetWorld(), MainWidgetClass);
		if(MainWidget)
		{
			MainWidget->AddToViewport();
		}
	}
}
