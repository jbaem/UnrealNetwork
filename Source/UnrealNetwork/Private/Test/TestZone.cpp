#include "Test/TestZone.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ATestZone::ATestZone()
{
	PrimaryActorTick.bCanEverTick = true;

	ZoneAreaComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ZoneAreaComponent"));
	ZoneAreaComponent->InitSphereRadius(500.f);
	RootComponent = ZoneAreaComponent;

	ZoneEffectComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ZoneEffectComponent"));

}

void ATestZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ATestZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATestZone::OnOverlapEnd);

}

void ATestZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Apply damage to overlapping actors every second
	FTimerHandle DamageTimerHandle;
	GetWorldTimerManager().SetTimer(
		DamageTimerHandle, 
		[this]()
			{
				for (AActor* Actor : OverlappingActors)
				{
					if (Actor)
					{
						UGameplayStatics::ApplyDamage(Actor, 10.f, nullptr, this, nullptr);
					}
				}
			}, 
		2.0f, 
		true
	);

}

void ATestZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingActors.AddUnique(OtherActor);
}

void ATestZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingActors.Remove(OtherActor);
}

