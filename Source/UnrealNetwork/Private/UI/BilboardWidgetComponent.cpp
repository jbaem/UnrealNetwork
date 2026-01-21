#include "UI/BilboardWidgetComponent.h"

void UBilboardWidgetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (PC && PC->PlayerCameraManager)
	{
		FVector CameraForward = PC->PlayerCameraManager->GetCameraRotation().Vector();
		FVector WidgetForward = CameraForward * -1.f;
		SetWorldRotation(WidgetForward.Rotation());
	}
}
