#include "Test/TestNetProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ATestNetProjectile::ATestNetProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(20.f);
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bShouldBounce = true;

}

void ATestNetProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator())
	{
		// Ignore the actor that fired the projectile
		Collision->IgnoreActorWhenMoving(GetInstigator(), true); 
	}

	OnActorHit.AddDynamic(this, &ATestNetProjectile::OnHit);
}

void ATestNetProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestNetProjectile::OnHit(AActor* SlefActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only the server should handle hit logic
	if(HasAuthority())
	{
		if(bHitProcessed || OtherActor == nullptr || OtherActor == GetInstigator())
		{
			return;
		}

		bHitProcessed = true;
		SetLifeSpan(2.0f);

		// Apply damage or other effects to the character
		if (OtherActor->IsA<ACharacter>())
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetInstigatorController(), this, UDamageType::StaticClass());

			// All clients should see the hit effect
			Multicast_OnHitEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
	}
}

void ATestNetProjectile::Multicast_OnHitEffect_Implementation(const FVector & InLocation, const FRotator & InRotation)
{
	if (HitEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, InLocation, InRotation);
	}
}

