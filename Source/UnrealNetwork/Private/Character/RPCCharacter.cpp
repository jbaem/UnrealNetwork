#include "Character/RPCCharacter.h"

ARPCCharacter::ARPCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GetMesh(), TEXT("Fire"));
}

void ARPCCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ARPCCharacter::OnTakeDamage);
}

void ARPCCharacter::Fire()
{
	// Only the owning client should call the server RPC
	if (IsLocallyControlled()) 
	{
		// Call the server RPC
		Server_Fire(); 
	}
}

void ARPCCharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%s took %f damage from %s"), *GetName(), Damage, *DamageCauser->GetName()));
		Client_OnHit();
	}
}

void ARPCCharacter::Server_Fire_Implementation()
{
	// Server-side logic for firing
	if (ProjectileClass && MuzzleLocation)
	{
		FVector SpawnLocation = MuzzleLocation->GetComponentLocation();
		FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		// Spawn the projectile on the server
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	}
}


void ARPCCharacter::Client_OnHit_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("You have been hit!"));
}
