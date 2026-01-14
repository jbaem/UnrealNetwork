#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TestZone.generated.h"

UCLASS()
class UNREALNETWORK_API ATestZone : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestZone();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void DamagingPerSecond();

	void DamageOverlappingActors();

	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	TObjectPtr<class USphereComponent> ZoneAreaComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	TObjectPtr<class UNiagaraComponent> ZoneEffectComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	TArray<TObjectPtr<AActor>> OverlappingActors;

private:
	FTimerHandle DamageTimerHandle;
};
