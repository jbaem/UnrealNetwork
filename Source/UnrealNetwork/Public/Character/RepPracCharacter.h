#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "RepPracCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLevelChangeDelegate, int32 , NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExpChangeDelegate, float, NewExp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangeDelegate, float, NewHealth);

UCLASS()
class UNREALNETWORK_API ARepPracCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	ARepPracCharacter();

	FOnLevelChangeDelegate OnLevelChange;
	FOnExpChangeDelegate OnExpChange;
	FOnHealthChangeDelegate OnHealthChange;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_OnTask1();
	UFUNCTION(Server, Reliable)
	void Server_OnTask2();
	UFUNCTION(Server, Reliable)
	void Server_OnTask3();

	UFUNCTION()
	void OnRepNotify_Level();
	UFUNCTION()
	void OnRepNotify_Exp();
	UFUNCTION()
	void OnRepNotify_Health();


	int32 GetLevel() const { return Level; }
	float GetExp() const { return Exp; }
	float GetHealth() const { return Health; }

protected:
	void OnTask1() { Server_OnTask1(); };
	void OnTask2() { Server_OnTask2(); };
	void OnTask3() { Server_OnTask3(); };
	
protected:
	UPROPERTY(ReplicatedUsing = OnRepNotify_Level)
	int32 Level = 1;
	UPROPERTY(ReplicatedUsing = OnRepNotify_Exp)
	float Exp = 0.0f;
	UPROPERTY(ReplicatedUsing = OnRepNotify_Health)
	float Health = 100.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> Task1Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> Task2Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> Task3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UWidgetComponent> WidgetComponent;
};
