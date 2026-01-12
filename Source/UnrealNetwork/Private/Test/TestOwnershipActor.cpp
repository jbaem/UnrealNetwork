#include "Test/TestOwnershipActor.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NetRoleCharacter.h"

ATestOwnershipActor::ATestOwnershipActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	Collision->SetupAttachment(Mesh);
	Collision->SetSphereRadius(400.f);

	bReplicates = true; // if networked, replicate this acto
}

void ATestOwnershipActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &ATestOwnershipActor::OnOverlapped);
}

void ATestOwnershipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugSphere(GetWorld(), GetActorLocation(), Collision->GetScaledSphereRadius(), 32, FColor::Yellow);

	if (HasAuthority())
	{
		AActor* NextOwner = nullptr;
		float MinSquaredDistance = Collision->GetScaledSphereRadius() * Collision->GetScaledSphereRadius();

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANetRoleCharacter::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			const float SquaredDistance = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());
			if (SquaredDistance < MinSquaredDistance)
			{
				MinSquaredDistance = SquaredDistance;
				NextOwner = Actor;
			}
		}
		if(GetOwner() != NextOwner)
		{
			SetOwner(NextOwner);
			FString OwnerName = NextOwner ? NextOwner->GetName() : TEXT("No Owner");
			UE_LOG(LogTemp, Warning, TEXT("ATestOwnershipActor::Tick: New Owner is %s"), *OwnerName);
		}
	}

	PrintNetworkInfo();

}


void ATestOwnershipActor::OnOverlapped(AActor* OverlappedActor, AActor* OtherActor)
{
	if (HasAuthority())
	{
		if(GetOwner() != OtherActor)
		{
			SetOwner(OtherActor);
			FString OwnerName = OtherActor ? OtherActor->GetName() : TEXT("No Owner");
			UE_LOG(LogTemp, Warning, TEXT("ATestOwnershipActor::OnOverlapped: New Owner is %s"), *OwnerName);
		}
	}
}

void ATestOwnershipActor::PrintNetworkInfo()
{
	const FString LocalRoleString = UEnum::GetValueAsString(GetLocalRole());
	const FString RemoteRoleString = UEnum::GetValueAsString(GetRemoteRole());

	const FString OwnerString = GetOwner() ? GetOwner()->GetName() : TEXT("No Owner");
	const FString ConnectionString = GetNetConnection() ? GetNetConnection()->GetName() : TEXT("No NetConnection");

	const FString NetworkInfo = FString::Printf(
		TEXT("[LocalRole]   %s\n[RemoteRole]   %s\n[Owner]   %s\n[NetConnection]   %s"),
		*LocalRoleString,
		*RemoteRoleString,
		*OwnerString,
		*ConnectionString
	);
	DrawDebugString(GetWorld(), GetActorLocation(), NetworkInfo, nullptr, FColor::White, 0.f, true);
}


