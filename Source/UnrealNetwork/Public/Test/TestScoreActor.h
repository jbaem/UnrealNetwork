#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TestScoreActor.generated.h"

UCLASS()
class UNREALNETWORK_API ATestScoreActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestScoreActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

protected:
	UPROPERTY(EditAnywhere, Category = "Test")
	TObjectPtr<class UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere, Category = "Test")
	TObjectPtr<class USphereComponent> Collision;
};
