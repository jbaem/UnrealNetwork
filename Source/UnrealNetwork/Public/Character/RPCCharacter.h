#pragma once

#include "CoreMinimal.h"
#include "UnrealNetworkCharacter.h"

#include "RPCCharacter.generated.h"

UCLASS()
class UNREALNETWORK_API ARPCCharacter : public AUnrealNetworkCharacter
{
	GENERATED_BODY()

public:
	ARPCCharacter();

protected:
	virtual void BeginPlay() override;
	
public:
	UFUNCTION(BlueprintCallable, Category = "NT|Test")
	void Fire();

protected:
	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(Client, Reliable)
	void Client_OnHit();

	UFUNCTION()
	void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<class USceneComponent> MuzzleLocation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NT|TestRPC")
	TSubclassOf<AActor> ProjectileClass = nullptr;
};
