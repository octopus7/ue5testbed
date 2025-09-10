// Skeleton-only component for driving animation state changes.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimSwitchTypes.h"
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

    UFUNCTION(BlueprintPure, Category = "AnimSwitch")
    FName GetCurrentState() const { return CurrentStateName; }

    UFUNCTION(BlueprintPure, Category = "AnimSwitch")
    FName GetRequestedState() const { return RequestedStateName; }

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    TMap<FName, TObjectPtr<UAnimationAsset>> Animations;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnimSwitch")
    FName InitialStateName = TEXT("Idle");

    UPROPERTY(BlueprintAssignable, Category = "AnimSwitch")
    FOnAnimStateChanged OnStateChanged;

protected:
    virtual void BeginPlay() override;

private:
    FName CurrentStateName;
    FName RequestedStateName;
};

