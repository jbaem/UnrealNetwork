#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMyScoreChanged, int32, NewScore);

UCLASS()
class UNREALNETWORK_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMyPlayerState();	

	FOnMyScoreChanged OnMyScoreChanged;

	UFUNCTION(BlueprintCallable, Category = "NT|Data")
	void AddMyScore(int32 InScore);
	UFUNCTION(BlueprintCallable, Category = "NT|Data")
	int32 GetMyScore() const { return MyScore; }
	
	UFUNCTION(BlueprintCallable, Category = "NT|Data")
	void SetMyName(const FString& InName);
	UFUNCTION(BlueprintCallable, Category = "NT|Data")
	FString GetMyName() const { return MyName; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MyScore();
	UFUNCTION()
	void OnRep_MyName();

	UPROPERTY(ReplicatedUsing = OnRep_MyScore, BlueprintReadOnly, Category = "NT|Data")
	int32 MyScore = 0;
	UPROPERTY(ReplicatedUsing = OnRep_MyName, BlueprintReadOnly, Category = "NT|Data")
	FString MyName;
};
