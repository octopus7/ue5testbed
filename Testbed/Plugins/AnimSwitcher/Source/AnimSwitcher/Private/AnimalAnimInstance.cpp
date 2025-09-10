#include "AnimalAnimInstance.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimSequenceBase.h"

UAnimMontage* UAnimalAnimInstance::PlayOneShot(UAnimationAsset* Asset, float BlendInTime, float BlendOutTime, float PlayRate)
{
    if (!Asset)
    {
        return nullptr;
    }
    const float InBlendIn = (BlendInTime >= 0.f) ? BlendInTime : DefaultOneShotBlendIn;
    const float InBlendOut = (BlendOutTime >= 0.f) ? BlendOutTime : DefaultOneShotBlendOut;

    if (UAnimSequenceBase* Seq = Cast<UAnimSequenceBase>(Asset))
    {
        return PlaySlotAnimationAsDynamicMontage(Seq, FullBodySlotName, InBlendIn, InBlendOut, /*InPlayRate=*/PlayRate, /*LoopCount=*/1);
    }
    if (UAnimMontage* Montage = Cast<UAnimMontage>(Asset))
    {
        Montage_Play(Montage, PlayRate);
        return Montage;
    }
    return nullptr;
}
