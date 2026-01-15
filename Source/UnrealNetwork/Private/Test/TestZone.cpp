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
	ZoneEffectComponent->SetupAttachment(RootComponent);
}

void ATestZone::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ATestZone::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ATestZone::OnOverlapEnd);

	DamagingPerSecond();
}

void ATestZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestZone::DamagingPerSecond()
{
	if(GetWorldTimerManager().IsTimerActive(DamageTimerHandle))
	{
		return;
	}

	//Apply damage to overlapping actors every second
	GetWorldTimerManager().SetTimer(
		DamageTimerHandle,
		this,
		&ATestZone::DamageOverlappingActors,
		1.0f,
		true
	);
}

void ATestZone::DamageOverlappingActors()
{
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor)
		{
			UGameplayStatics::ApplyDamage(Actor, 10.f, nullptr, this, nullptr);
		}
	}
}

void ATestZone::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingActors.AddUnique(OtherActor);
}

void ATestZone::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	OverlappingActors.Remove(OtherActor);
}

