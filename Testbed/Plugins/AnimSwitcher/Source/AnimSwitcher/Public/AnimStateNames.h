// Canonical animation state keys without asset-specific prefixes (e.g., "Bird_")

#pragma once

#include "CoreMinimal.h"

namespace AnimState
{
    inline const FName Idle         { TEXT("Idle") };
    inline const FName IdleSitting  { TEXT("IdleSitting") };
    inline const FName IdleVariation{ TEXT("IdleVariation") };

    // Locomotion (land)
    inline const FName WalkForward  { TEXT("WalkForward") };
    inline const FName WalkBackwards{ TEXT("WalkBackwards") };
    inline const FName RunForward   { TEXT("RunForward") };
    inline const FName Turn90L      { TEXT("Turn90L") };
    inline const FName Turn90R      { TEXT("Turn90R") };

    // Locomotion (swim)
    inline const FName SwimIdle     { TEXT("SwimIdle") };
    inline const FName SwimForward  { TEXT("SwimForward") };
    inline const FName SwimEating   { TEXT("SwimEating") };

    // Actions / reactions
    inline const FName Attack01     { TEXT("Attack01") };
    inline const FName Eating       { TEXT("Eating") };
    inline const FName HitReaction  { TEXT("HitReaction") };
    inline const FName Death        { TEXT("Death") };

    // Posture transitions
    inline const FName StandToSit   { TEXT("StandToSit") };
    inline const FName SitToStand   { TEXT("SitToStand") };
}

