#include "Framework/TestMultiplayGameInstance.h"

#include "Kismet/GameplayStatics.h"

UTestMultiplayGameInstance::UTestMultiplayGameInstance()
{
	ServerIP = "127.0.0.1";

}

void UTestMultiplayGameInstance::Init()
{
	Super::Init();

	if(UEngine* Engine = GetEngine())
	{
		Engine->OnNetworkFailure().AddUObject(this, &UTestMultiplayGameInstance::HandleNetworkFailued);
		Engine->OnTravelFailure().AddUObject(this, &UTestMultiplayGameInstance::HandleTravelFailure);
	}
}

void UTestMultiplayGameInstance::CreateServer()
{
	FString LevelName = 
		ServerLevelAsset.IsNull() ?
		UGameplayStatics::GetCurrentLevelName(GetWorld()) :
		ServerLevelAsset.GetLongPackageName();

	UWorld* World = GetWorld();
	if(!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::CreateServer: World is null"));
		return;
	}

	FString Options = FString::Printf(TEXT("listen?MaxPlayers=%d"), MaxPlayers);
	UE_LOG(LogTemp, Log, TEXT("UTestMultiplayGameInstance::CreateServer: Traveling to %s with options %s"), *LevelName, *Options);

	UGameplayStatics::OpenLevel(World, FName(*LevelName), true, Options);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Server created at %s"), *ServerIP));
	}

}

void UTestMultiplayGameInstance::JoinServer(FString IPAddress)
{
	if(IPAddress.IsEmpty())
	{
		IPAddress = ServerIP;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::JoinServer: World is null"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::JoinServer: PlayerController is null"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("UTestMultiplayGameInstance::JoinServer: Joining server at %s"), *IPAddress);
	
	PC->ClientTravel(IPAddress, TRAVEL_Absolute);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining server at %s"), *IPAddress));
	}
}

void UTestMultiplayGameInstance::DisconnectServer()
{
	FString LevelName;
	if(MainLevelAsset.IsNull())
	{
		UE_LOG(LogTemp, Warning, TEXT("UTestMultiplayGameInstance::DisconnectServer: MainLevelAsset is null, cannot disconnect to main level"));
		return;
	}
	else
	{
		LevelName = MainLevelAsset.GetLongPackageName();
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::DisconnectServer: World is null"));
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC)
	{
		UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::DisconnectServer: PlayerController is null"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("UTestMultiplayGameInstance::DisconnectServer: Traveling to %s"), *LevelName);

	PC->ClientTravel(LevelName, TRAVEL_Absolute); // 1.LevelName, 2.IPAddress
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Disconnecting to main level %s"), *LevelName));
	}
}

void UTestMultiplayGameInstance::HandleNetworkFailued(UWorld* InWorld, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::HandleNetworkFailued: Network failure occurred. \nType: %d, \nError: %s"), static_cast<int32>(FailureType), *ErrorString);

}


void UTestMultiplayGameInstance::HandleTravelFailure(UWorld* InWorld, ETravelFailure::Type FailureType, const FString& ErrorString)
{
	UE_LOG(LogTemp, Error, TEXT("UTestMultiplayGameInstance::HandleTravelFailure: Travel failure occurred. \nType: %d, \nError: %s"), static_cast<int32>(FailureType), *ErrorString);

	switch (FailureType)
	{
	case ETravelFailure::ServerTravelFailure:
		if (ErrorString.Contains("Full") || ErrorString.Contains("full"))
		{
			UE_LOG(LogTemp, Warning, TEXT("UTestMultiplayGameInstance::HandleTravelFailure: Server is full."));
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Cannot join server: Server is full.")));
			}
		}
		break;
	default:
		break;
	}
}
