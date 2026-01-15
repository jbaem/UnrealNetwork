#include "Test/TestScoreActor.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

#include "GameFramework/Character.h"
#include "Framework/MyPlayerState.h"

ATestScoreActor::ATestScoreActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetRelativeLocation(FVector::UpVector * 50.f);
}

void ATestScoreActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		OnActorBeginOverlap.AddDynamic(this, &ATestScoreActor::OnOverlapBegin);
	}
}

void ATestScoreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestScoreActor::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		ACharacter* Character = Cast<ACharacter>(OtherActor);
		if (Character)
		{
			AMyPlayerState* PS = Character->GetPlayerState<AMyPlayerState>();
		
			if (PS)
			{
				PS->AddMyScore(10);
				Destroy();
			}
		}
	}
}
