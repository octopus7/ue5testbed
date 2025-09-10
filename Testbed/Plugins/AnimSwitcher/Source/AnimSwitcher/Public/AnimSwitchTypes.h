// Basic types for animation switching

#pragma once

#include "CoreMinimal.h"
#include "AnimSwitchTypes.generated.h"

UENUM(BlueprintType)
enum class EAnimSituation : uint8
{
    Idle       UMETA(DisplayName = "Idle"),
    Walk       UMETA(DisplayName = "Walk"),
    Run        UMETA(DisplayName = "Run"),
    Attack     UMETA(DisplayName = "Attack"),
    Hit        UMETA(DisplayName = "Hit"),
    Death      UMETA(DisplayName = "Death"),
    CustomA    UMETA(DisplayName = "Custom A"),
    CustomB    UMETA(DisplayName = "Custom B")
};

