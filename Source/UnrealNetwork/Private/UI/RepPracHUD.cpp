#include "UI/RepPracHUD.h"

#include "UI/RepPracMainWidget.h"
#include "GameFramework/PlayerController.h"
#include "Character/RepPracCharacter.h"

void ARepPracHUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass)
	{
		MainWidget = CreateWidget<URepPracMainWidget>(GetOwningPlayerController(), WidgetClass);
		MainWidget->AddToViewport();
	}

	if (APlayerController* PC = GetOwningPlayerController())
	{
		if (ARepPracCharacter* Character = Cast<ARepPracCharacter>(PC->GetPawn()))
		{
			Character->OnLevelChange.AddDynamic(MainWidget, &URepPracMainWidget::UpdateLevelDisplay);
			Character->OnExpChange.AddDynamic(MainWidget, &URepPracMainWidget::UpdateExpDisplay);

			MainWidget->UpdateLevelDisplay(Character->GetLevel());
			MainWidget->UpdateExpDisplay(Character->GetExp());
		}
	}
}
