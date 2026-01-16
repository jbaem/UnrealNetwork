#include "UI/CreateServerMainWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

#include "Kismet/GameplayStatics.h"
#include "Framework/TestMultiplayGameInstance.h"

void UCreateServerMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_Create)
	{
		Btn_Create->OnClicked.AddDynamic(this, &UCreateServerMainWidget::OnCreateButtonClicked);
	}

	if (Btn_Join)
	{
		Btn_Join->OnClicked.AddDynamic(this, &UCreateServerMainWidget::OnJoinButtenClicked);
	}
	
	if (Btn_Disconnect)
	{
		Btn_Disconnect->OnClicked.AddDynamic(this, &UCreateServerMainWidget::OnDisconnectButtenClicked);
	}
}

void UCreateServerMainWidget::OnCreateButtonClicked()
{
	UTestMultiplayGameInstance* GI = Cast<UTestMultiplayGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GI)
	{
		GI->CreateServer();
		SetButtonEnabled(false);
	}
}

void UCreateServerMainWidget::OnJoinButtenClicked()
{
	UTestMultiplayGameInstance* GI = Cast<UTestMultiplayGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		FString IPAddress = GI->GetServerIP();
		if (IPText)
		{
			FString InputIP = IPText->GetText().ToString();
			if (!InputIP.IsEmpty())
			{
				IPAddress = InputIP;
			}
		}

		GI->JoinServer(IPAddress);
		SetButtonEnabled(false);
	}
}

void UCreateServerMainWidget::OnDisconnectButtenClicked()
{
	UTestMultiplayGameInstance* GI = Cast<UTestMultiplayGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI)
	{
		GI->DisconnectServer();
		SetButtonEnabled(true);
	}
}

void UCreateServerMainWidget::SetButtonEnabled(bool InIsMainLevel)
{
	Btn_Create->SetIsEnabled(InIsMainLevel);
	Btn_Join->SetIsEnabled(InIsMainLevel);
	Btn_Disconnect->SetIsEnabled(!InIsMainLevel);
}
