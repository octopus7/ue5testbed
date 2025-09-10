// Strongly-typed set of animation assets used by AnimSwitchComponent

#pragma once

#include "CoreMinimal.h"
#include "AnimalAnimSet.generated.h"

class UAnimationAsset;

USTRUCT(BlueprintType)
struct FAnimalAnimSet
{
    GENERATED_BODY()

    // Idle
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Idle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> IdleSitting = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> IdleVariation = nullptr;

    // Locomotion - Land
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> WalkForward = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> WalkBackwards = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> RunForward = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Turn90L = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Turn90R = nullptr;

    // Locomotion - Swim
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> SwimIdle = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> SwimForward = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> SwimEating = nullptr;

    // Actions
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Attack01 = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Eating = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> HitReaction = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> Death = nullptr;

    // Posture transitions
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> StandToSit = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UAnimationAsset> SitToStand = nullptr;
};

