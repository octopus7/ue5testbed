// An actor that spawns a specified UMG widget to control an AnimSwitchComponent at runtime.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AnimSwitcherUIDriver.generated.h"

class UUserWidget;

UCLASS()
class ANIMSWITCHER_API AAnimSwitcherUIDriver : public AActor
{
    GENERATED_BODY()

public:
    AAnimSwitcherUIDriver();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|UI")
    TSubclassOf<UUserWidget> UIClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|UI")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|UI")
    int32 ZOrder = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|UI")
    bool bShowMouseCursor = true;

private:
    UPROPERTY(Transient)
    TObjectPtr<UUserWidget> SpawnedWidget;
};

