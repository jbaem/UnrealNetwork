#include "Character/PSCharacter.h"

#include "Net/UnrealNetwork.h"
#include "Framework/MyPlayerState.h"
#include "Components/WidgetComponent.h"

#include "UI/DisplayValuesWidget.h"

APSCharacter::APSCharacter()
{
	bReplicates = true;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(RootComponent);
}

void APSCharacter::BeginPlay()
{
	Super::BeginPlay();

	ScoreWidget = Cast<UDisplayValuesWidget>(WidgetComp->GetWidget());
	ScoreWidget->SetNameText(TEXT("Score: "));

	if(HasAuthority())
	{
		BindPlayerState();
		Score = 0;
	}
}

void APSCharacter::BindPlayerState()
{
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->OnMyScoreChanged.AddUniqueDynamic(this, &APSCharacter::UpdateScore);
		Score = PS->GetMyScore();
		UpdateScore(PS->GetMyScore());
	}
}

void APSCharacter::Tick(float DeltaTime)
{
	if (WidgetComp)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC && PC->PlayerCameraManager)
		{
			FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();
			FRotator LookAtRotation = FRotationMatrix::MakeFromX(-CameraForward).Rotator();
			WidgetComp->SetWorldRotation(LookAtRotation);
		}
	}
}

void APSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APSCharacter, Score);
}

void APSCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	BindPlayerState();
}

void APSCharacter::Server_AddScore_Implementation(int32 Point)
{
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->AddMyScore(Point);
	}
}

void APSCharacter::OnRep_Score()
{
	if (!ScoreWidget.IsValid())
	{
		ScoreWidget = Cast<UDisplayValuesWidget>(WidgetComp->GetWidget());
	}

	if (ScoreWidget.IsValid())
	{
		ScoreWidget->SetValueText(Score);
	}
}

void APSCharacter::Server_SetMyName_Implementation(const FString& InName)
{
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		PS->SetMyName(InName);
	}
}

void APSCharacter::TestAddScore(int32 Point)
{
	if(IsLocallyControlled())
	{
		Server_AddScore(Point);
	}
}

void APSCharacter::UpdateScore(int32 NewScore)
{
	if(HasAuthority())
	{
		Score = NewScore;
		OnRep_Score();
	}
}

