#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TestOwnershipActor.generated.h"

UCLASS()
class UNREALNETWORK_API ATestOwnershipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestOwnershipActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapped(AActor* OverlappedActor, AActor* OtherActor);

private:
	void PrintNetworkInfo();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|Test")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|Test")
	TObjectPtr<class USphereComponent> Collision;

};
