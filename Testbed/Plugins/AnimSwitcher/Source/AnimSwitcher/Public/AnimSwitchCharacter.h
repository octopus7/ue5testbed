// Character that owns AnimSwitchComponent. No player logic.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimSwitchCharacter.generated.h"

class UAnimSwitchComponent;

UCLASS(Blueprintable, BlueprintType)
class ANIMSWITCHER_API AAnimSwitchCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAnimSwitchCharacter();

    UFUNCTION(BlueprintPure, Category = "AnimSwitch")
    UAnimSwitchComponent* GetAnimSwitchComponent() const { return AnimSwitchComponent; }

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAnimSwitchComponent* AnimSwitchComponent;
};

