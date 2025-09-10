#include "AnimSwitchComponent.h"

#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

UAnimSwitchComponent::UAnimSwitchComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // structure-only: no ticking needed yet
}

void UAnimSwitchComponent::BeginPlay()
{
    Super::BeginPlay();

    if (!TargetMesh)
    {
        if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
        {
            TargetMesh = Char->GetMesh();
        }
        else if (GetOwner())
        {
            TargetMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
        }
    }

    // Structure only: set internal state, do not apply animations yet
    CurrentStateName = InitialStateName;
    RequestedStateName = NAME_None;
}

void UAnimSwitchComponent::InitializeWithMesh(USkeletalMeshComponent* InMesh)
{
    TargetMesh = InMesh;
}

void UAnimSwitchComponent::RequestStateByName(FName StateName, float /*BlendTime*/)
{
    // Structure-only: stash requested state, do not play
    RequestedStateName = StateName;
}

void UAnimSwitchComponent::RequestSituation(EAnimSituation Situation, float /*BlendTime*/)
{
    // Structure-only: convert enum to name and forward to RequestStateByName
    const FName StateName = StaticEnum<EAnimSituation>()->GetNameByValue((int64)Situation);
    RequestStateByName(StateName);
}

