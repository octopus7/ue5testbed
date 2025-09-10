#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimalAnimInstance.generated.h"

UCLASS(Blueprintable)
class ANIMSWITCHER_API UAnimalAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;

    // Drive locomotion blending in ABP state machine
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    float Speed = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bSwimming = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bSitting = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
    bool bDead = false;

    // Slot and defaults for one-shots
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
    FName FullBodySlotName = TEXT("DefaultSlot");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
    float DefaultOneShotBlendIn = 0.12f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim|Montage")
    float DefaultOneShotBlendOut = 0.12f;

public:
    UFUNCTION(BlueprintCallable, Category = "Anim|Runtime")
    void SetLocomotionVars(float InSpeed, bool bInSwimming, bool bInSitting, bool bInDead)
    {
        Speed = InSpeed; bSwimming = bInSwimming; bSitting = bInSitting; bDead = bInDead;
    }

    UFUNCTION(BlueprintCallable, Category = "Anim|Runtime")
    UAnimMontage* PlayOneShot(UAnimationAsset* Asset, float BlendInTime = -1.f, float BlendOutTime = -1.f, float PlayRate = 1.f);
};
