#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "CreateServerMainWidget.generated.h"

UCLASS()
class UNREALNETWORK_API UCreateServerMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnCreateButtonClicked();
	UFUNCTION()
	void OnJoinButtenClicked();
	UFUNCTION()
	void OnDisconnectButtenClicked();

private:
	void SetButtonEnabled(bool InIsMainLevel);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Create;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Join;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Btn_Disconnect;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> IPText;

};
