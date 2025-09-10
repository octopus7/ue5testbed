#include "AnimSwitcherUIDriver.h"

#include "AnimSwitcherControlWidget.h"
#include "AnimSwitchComponent.h"
#include "Blueprint/UserWidget.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerController.h"

AAnimSwitcherUIDriver::AAnimSwitcherUIDriver()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AAnimSwitcherUIDriver::BeginPlay()
{
    Super::BeginPlay();

    if (!UIClass)
    {
        return;
    }

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        return;
    }

    SpawnedWidget = CreateWidget<UUserWidget>(PC, UIClass);
    if (!SpawnedWidget)
    {
        return;
    }
    SpawnedWidget->AddToViewport(ZOrder);

    if (bShowMouseCursor)
    {
        PC->bShowMouseCursor = true;
        FInputModeGameAndUI Mode;
        Mode.SetHideCursorDuringCapture(false);
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PC->SetInputMode(Mode);
    }

    if (UAnimSwitcherControlWidget* Control = Cast<UAnimSwitcherControlWidget>(SpawnedWidget))
    {
        if (TargetActor)
        {
            Control->SetTargetActor(TargetActor);
        }
        else
        {
            // Fallback: pick first actor with AnimSwitchComponent
            for (TActorIterator<AActor> It(GetWorld()); It; ++It)
            {
                if (It->FindComponentByClass<UActorComponent>())
                {
                    if (It->FindComponentByClass<UAnimSwitchComponent>())
                    {
                        Control->SetTargetActor(*It);
                        break;
                    }
                }
            }
        }
    }
}
