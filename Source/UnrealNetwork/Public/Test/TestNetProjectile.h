#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TestNetProjectile.generated.h"

UCLASS()
class UNREALNETWORK_API ATestNetProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ATestNetProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(AActor* SlefActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnHitEffect(const FVector& InLocation, const FRotator& InRotation);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USphereComponent> Collision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> Mesh = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	TObjectPtr<class UNiagaraSystem> HitEffect = nullptr;

private:
	bool bHitProcessed = false;

};
