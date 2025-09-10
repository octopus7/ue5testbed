// Skeleton-only component for driving animation state changes.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimSwitchTypes.h"
#include "AnimalAnimSet.h"
#include "TimerManager.h"
#include "AnimSwitchComponent.generated.h"

class USkeletalMeshComponent;
class UAnimationAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimStateChanged, FName, PreviousState, FName, NewState);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANIMSWITCHER_API UAnimSwitchComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAnimSwitchComponent();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch")
    void InitializeWithMesh(USkeletalMeshComponent* InMesh);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch")
    void RequestStateByName(FName StateName, float BlendTime = 0.2f);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch")
    void RequestSituation(EAnimSituation Situation, float BlendTime = 0.2f);

    // Setup helpers (structure only): register animations using canonical keys derived from asset names.
    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Setup")
    void AddAnimationAsset(UAnimationAsset* Asset, bool bOverrideExisting = true);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Setup")
    void AddAnimationAssets(const TArray<UAnimationAsset*>& Assets, bool bOverrideExisting = true);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Setup")
    void SetAnimationForKey(FName Key, UAnimationAsset* Asset, bool bOverrideExisting = true);

    UFUNCTION(BlueprintPure, Category = "AnimSwitch|Setup")
    static FName CanonicalKeyFromAssetName(const FString& AssetName, const FString& OptionalPrefixToStrip = TEXT("Bird_"));

    UFUNCTION(BlueprintPure, Category = "AnimSwitch")
    FName GetCurrentState() const { return CurrentStateName; }

    UFUNCTION(BlueprintPure, Category = "AnimSwitch")
    FName GetRequestedState() const { return RequestedStateName; }

    // Runtime control (actions & state)
    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void SetSitting(bool bSit);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestSit();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestStand();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void SetSwimming(bool bSwim);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestAttack();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestEat();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestHit();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestDie();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestTurnLeft90();

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|Runtime")
    void RequestTurnRight90();

    UFUNCTION(BlueprintPure, Category = "AnimSwitch|Runtime")
    bool IsSitting() const { return bIsSitting; }

    UFUNCTION(BlueprintPure, Category = "AnimSwitch|Runtime")
    bool IsSwimming() const { return bIsSwimming; }

    UFUNCTION(BlueprintPure, Category = "AnimSwitch|Runtime")
    bool IsDead() const { return bIsDead; }

public:
    // Strongly-typed asset set for easy assignment in BP
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|Assets")
    FAnimalAnimSet AnimSet;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    TMap<FName, TObjectPtr<UAnimationAsset>> Animations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    FName InitialStateName = TEXT("Idle");

    UPROPERTY(BlueprintAssignable, Category = "AnimSwitch")
    FOnAnimStateChanged OnStateChanged;

    // Prefix to strip from asset names when generating keys (e.g., "Bird_")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|Setup")
    FString AssetPrefixToStrip = TEXT("Bird_");

    // Control thresholds
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|Runtime")
    float WalkSpeedThreshold = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|Runtime")
    float RunSpeedThreshold = 300.f;

    // When true, component switches mesh to SingleNode mode to directly drive sequences
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimSwitch|Runtime")
    bool bForceSingleNodeMode = true;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FName CurrentStateName;
    FName RequestedStateName;

    // Runtime flags
    bool bIsSitting = false;
    bool bIsSwimming = false;
    bool bIsDead = false;

    // Loop/oneshot bookkeeping
    bool bPlayingOneShot = false;
    FName CurrentLoopKey;
    FTimerHandle OneShotTimerHandle;

private:
    float GetOwnerSpeed() const;
    FName ChooseBaseLoopKey(float Speed) const;
    void ApplyBaseLoopIfNeeded(FName DesiredKey);
    UAnimationAsset* GetAssetByKey(const FName& Key) const;
    void PlayOneShot(UAnimationAsset* Asset, TFunction<void()> OnFinished = nullptr, float PlayRate = 1.f);
    void ResumeBaseLoop();
};
