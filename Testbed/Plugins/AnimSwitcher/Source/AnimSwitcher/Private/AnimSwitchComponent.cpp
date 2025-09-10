#include "AnimSwitchComponent.h"

#include "Animation/AnimationAsset.h"
#include "Animation/AnimSequenceBase.h"
#include "AnimalAnimInstance.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

UAnimSwitchComponent::UAnimSwitchComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // runtime: evaluate speed and manage loops
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

    // Setup mesh reference if missing
    if (TargetMesh)
    {
        if (bForceSingleNodeMode)
        {
            TargetMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        }
    }

    // Initialize state
    CurrentStateName = InitialStateName;
    RequestedStateName = NAME_None;

    ResumeBaseLoop();
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

void UAnimSwitchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!TargetMesh)
    {
        return;
    }
    if (bIsDead)
    {
        return; // freeze on death pose
    }
    const float Speed = GetOwnerSpeed();

    // If using AnimInstance (preferred for blending), just feed variables and let ABP handle the cross-fade
    if (UAnimalAnimInstance* AI = Cast<UAnimalAnimInstance>(TargetMesh->GetAnimInstance()))
    {
        AI->SetLocomotionVars(Speed, bIsSwimming, bIsSitting, bIsDead);
    }
    else
    {
        // Fallback: single node (no blend)
        const FName DesiredKey = ChooseBaseLoopKey(Speed);
        ApplyBaseLoopIfNeeded(DesiredKey);
    }
}

float UAnimSwitchComponent::GetOwnerSpeed() const
{
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return 0.f;
    }
    const FVector Vel = Owner->GetVelocity();
    // Horizontal speed (ignore vertical for simplicity)
    return FVector(Vel.X, Vel.Y, 0.f).Size();
}

FName UAnimSwitchComponent::ChooseBaseLoopKey(float Speed) const
{
    if (bIsDead)
    {
        return NAME_None;
    }

    if (bIsSitting)
    {
        return FName(TEXT("IdleSitting"));
    }

    if (bIsSwimming)
    {
        if (Speed > WalkSpeedThreshold)
        {
            return FName(TEXT("SwimForward"));
        }
        return FName(TEXT("SwimIdle"));
    }

    if (Speed > RunSpeedThreshold)
    {
        return FName(TEXT("RunForward"));
    }
    if (Speed > WalkSpeedThreshold)
    {
        return FName(TEXT("WalkForward"));
    }
    return FName(TEXT("Idle"));
}

void UAnimSwitchComponent::ApplyBaseLoopIfNeeded(FName DesiredKey)
{
    if (bPlayingOneShot || DesiredKey.IsNone())
    {
        return;
    }
    if (DesiredKey == CurrentLoopKey)
    {
        return; // do not restart same loop each tick
    }
    if (!TargetMesh)
    {
        return;
    }
    UAnimationAsset* Asset = GetAssetByKey(DesiredKey);
    if (!Asset)
    {
        return;
    }

    if (bForceSingleNodeMode)
    {
        TargetMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
    }
    TargetMesh->PlayAnimation(Asset, /*bLoop=*/true);
    CurrentLoopKey = DesiredKey;
}

UAnimationAsset* UAnimSwitchComponent::GetAssetByKey(const FName& Key) const
{
    const FString K = Key.ToString();

    // Prefer typed set
    if (K == TEXT("Idle")) return AnimSet.Idle;
    if (K == TEXT("IdleSitting")) return AnimSet.IdleSitting;
    if (K == TEXT("IdleVariation")) return AnimSet.IdleVariation;
    if (K == TEXT("WalkForward")) return AnimSet.WalkForward;
    if (K == TEXT("WalkBackwards")) return AnimSet.WalkBackwards;
    if (K == TEXT("RunForward")) return AnimSet.RunForward;
    if (K == TEXT("Turn90L")) return AnimSet.Turn90L;
    if (K == TEXT("Turn90R")) return AnimSet.Turn90R;
    if (K == TEXT("SwimIdle")) return AnimSet.SwimIdle;
    if (K == TEXT("SwimForward")) return AnimSet.SwimForward;
    if (K == TEXT("SwimEating")) return AnimSet.SwimEating;
    if (K == TEXT("Attack01")) return AnimSet.Attack01;
    if (K == TEXT("Eating")) return AnimSet.Eating;
    if (K == TEXT("HitReaction")) return AnimSet.HitReaction;
    if (K == TEXT("Death")) return AnimSet.Death;
    if (K == TEXT("StandToSit")) return AnimSet.StandToSit;
    if (K == TEXT("SitToStand")) return AnimSet.SitToStand;

    // Fallback to map by key
    if (const TObjectPtr<UAnimationAsset>* Found = Animations.Find(Key))
    {
        return Found->Get();
    }
    return nullptr;
}

void UAnimSwitchComponent::PlayOneShot(UAnimationAsset* Asset, TFunction<void()> OnFinished, float PlayRate)
{
    if (!TargetMesh || !Asset)
    {
        return;
    }
    if (UAnimalAnimInstance* AI = Cast<UAnimalAnimInstance>(TargetMesh->GetAnimInstance()))
    {
        bPlayingOneShot = true;
        if (UAnimMontage* M = AI->PlayOneShot(Asset))
        {
            // Bind to montage end to resume base loop
            FOnMontageEnded EndDelegate;
            EndDelegate.BindLambda([this, OnFinished](UAnimMontage*, bool /*bInterrupted*/)
            {
                if (OnFinished) { OnFinished(); }
                bPlayingOneShot = false;
                ResumeBaseLoop();
            });
            AI->Montage_SetEndDelegate(EndDelegate, M);
        }
        else
        {
            bPlayingOneShot = false;
        }
    }
    else
    {
        // Fallback: Single node without blend
        if (bForceSingleNodeMode)
        {
            TargetMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        }
        bPlayingOneShot = true;
        TargetMesh->PlayAnimation(Asset, /*bLoop=*/false);
        if (UAnimSequenceBase* Seq = Cast<UAnimSequenceBase>(Asset))
        {
            const float Len = Seq->GetPlayLength();
            FTimerDelegate Del = FTimerDelegate::CreateLambda([this, OnFinished]()
            {
                if (OnFinished) { OnFinished(); }
                bPlayingOneShot = false;
                ResumeBaseLoop();
            });
            GetWorld()->GetTimerManager().SetTimer(OneShotTimerHandle, Del, Len / FMath::Max(PlayRate, 0.01f), /*bLoop=*/false);
        }
    }
}

void UAnimSwitchComponent::ResumeBaseLoop()
{
    if (bIsDead)
    {
        return; // keep death pose
    }
    const float Speed = GetOwnerSpeed();
    const FName Desired = ChooseBaseLoopKey(Speed);
    ApplyBaseLoopIfNeeded(Desired);
}

void UAnimSwitchComponent::SetSitting(bool bSit)
{
    if (bIsDead)
    {
        return;
    }
    if (bSit == bIsSitting)
    {
        return;
    }
    if (bSit)
    {
        if (UAnimationAsset* Trans = GetAssetByKey(FName(TEXT("StandToSit"))))
        {
            PlayOneShot(Trans, [this]() { bIsSitting = true; }, 1.f);
            return;
        }
        bIsSitting = true;
    }
    else
    {
        if (UAnimationAsset* Trans = GetAssetByKey(FName(TEXT("SitToStand"))))
        {
            PlayOneShot(Trans, [this]() { bIsSitting = false; }, 1.f);
            return;
        }
        bIsSitting = false;
    }
    ResumeBaseLoop();
}

void UAnimSwitchComponent::RequestSit() { SetSitting(true); }
void UAnimSwitchComponent::RequestStand() { SetSitting(false); }

void UAnimSwitchComponent::SetSwimming(bool bSwim)
{
    if (bIsDead)
    {
        return;
    }
    bIsSwimming = bSwim;
    ResumeBaseLoop();
}

void UAnimSwitchComponent::RequestAttack()
{
    if (bIsDead) return;
    if (UAnimationAsset* A = GetAssetByKey(FName(TEXT("Attack01"))))
    {
        PlayOneShot(A);
    }
}

void UAnimSwitchComponent::RequestEat()
{
    if (bIsDead) return;
    UAnimationAsset* A = nullptr;
    if (bIsSwimming)
    {
        A = GetAssetByKey(FName(TEXT("SwimEating")));
    }
    if (!A)
    {
        A = GetAssetByKey(FName(TEXT("Eating")));
    }
    if (A)
    {
        PlayOneShot(A);
    }
}

void UAnimSwitchComponent::RequestHit()
{
    if (bIsDead) return;
    if (UAnimationAsset* A = GetAssetByKey(FName(TEXT("HitReaction"))))
    {
        PlayOneShot(A);
    }
}

void UAnimSwitchComponent::RequestDie()
{
    if (bIsDead) return;
    bIsDead = true;
    if (UAnimationAsset* A = GetAssetByKey(FName(TEXT("Death"))))
    {
        // After death finishes, do not resume base loop
        PlayOneShot(A, /*OnFinished=*/nullptr);
    }
}

void UAnimSwitchComponent::RequestTurnLeft90()
{
    if (bIsDead) return;
    if (UAnimationAsset* A = GetAssetByKey(FName(TEXT("Turn90L"))))
    {
        PlayOneShot(A);
    }
}

void UAnimSwitchComponent::RequestTurnRight90()
{
    if (bIsDead) return;
    if (UAnimationAsset* A = GetAssetByKey(FName(TEXT("Turn90R"))))
    {
        PlayOneShot(A);
    }
}

FName UAnimSwitchComponent::CanonicalKeyFromAssetName(const FString& AssetName, const FString& OptionalPrefixToStrip)
{
    FString Name = AssetName;
    if (!OptionalPrefixToStrip.IsEmpty() && Name.StartsWith(OptionalPrefixToStrip, ESearchCase::IgnoreCase))
    {
        Name.RightChopInline(OptionalPrefixToStrip.Len(), /*bAllowShrinking=*/false);
    }

    TArray<FString> Tokens;
    Name.ParseIntoArray(Tokens, TEXT("_"), /*CullEmpty=*/true);

    FString Canonical;
    Canonical.Reserve(Name.Len());

    for (FString& Token : Tokens)
    {
        if (Token.IsEmpty())
        {
            continue;
        }

        // Keep pure numeric tokens as-is (e.g., "90")
        bool bAllDigits = true;
        for (int32 i = 0; i < Token.Len(); ++i)
        {
            if (!FChar::IsDigit(Token[i])) { bAllDigits = false; break; }
        }
        if (bAllDigits)
        {
            Canonical += Token; // preserve
            continue;
        }

        FString Lower = Token.ToLower();
        if (Lower == TEXT("l")) { Canonical += TEXT("L"); continue; }
        if (Lower == TEXT("r")) { Canonical += TEXT("R"); continue; }

        // Title-case: upper first char, lower rest
        Lower[0] = TChar<TCHAR>::ToUpper(Lower[0]);
        Canonical += Lower;
    }

    return FName(Canonical);
}

void UAnimSwitchComponent::AddAnimationAsset(UAnimationAsset* Asset, bool bOverrideExisting)
{
    if (!Asset)
    {
        return;
    }
    const FName Key = CanonicalKeyFromAssetName(Asset->GetName(), AssetPrefixToStrip);
    SetAnimationForKey(Key, Asset, bOverrideExisting);
}

void UAnimSwitchComponent::AddAnimationAssets(const TArray<UAnimationAsset*>& Assets, bool bOverrideExisting)
{
    for (UAnimationAsset* Asset : Assets)
    {
        AddAnimationAsset(Asset, bOverrideExisting);
    }
}

void UAnimSwitchComponent::SetAnimationForKey(FName Key, UAnimationAsset* Asset, bool bOverrideExisting)
{
    if (!Asset || Key.IsNone())
    {
        return;
    }
    if (!bOverrideExisting && Animations.Contains(Key))
    {
        return;
    }
    Animations.FindOrAdd(Key) = Asset;
}
