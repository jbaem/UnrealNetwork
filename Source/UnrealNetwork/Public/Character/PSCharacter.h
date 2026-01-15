#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"
#include "PSCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API APSCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	APSCharacter();

protected:
	virtual void BeginPlay() override;
	void BindPlayerState();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnRep_PlayerState() override;

	UFUNCTION(Server, Reliable)
	void Server_AddScore(int32 Point);

	UFUNCTION(Server, Reliable)
	void Server_SetMyName(const FString& InName);

public:
	UFUNCTION(BlueprintCallable, Category = "NT|Test")
	void TestAddScore(int32 Point);

	UFUNCTION()
	void UpdateScore(int32 NewScore);

	UFUNCTION()
	void OnRep_Score();

protected:
	UPROPERTY(EditAnywhere, Category = "NT|UI")
	TObjectPtr<class UWidgetComponent> WidgetComp;
	UPROPERTY()
	TWeakObjectPtr<class UDisplayValuesWidget> ScoreWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_Score)
	int32 Score = 0;
};
