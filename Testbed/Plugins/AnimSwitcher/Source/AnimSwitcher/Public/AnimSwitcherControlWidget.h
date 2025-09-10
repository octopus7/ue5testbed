// A minimal UMG control widget that triggers AnimSwitchComponent actions via buttons.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimSwitcherControlWidget.generated.h"

class UButton;
class UAnimSwitchComponent;

UCLASS(BlueprintType)
class ANIMSWITCHER_API UAnimSwitcherControlWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|UI")
    void SetTargetActor(AActor* InActor);

    UFUNCTION(BlueprintCallable, Category = "AnimSwitch|UI")
    void SetTargetComponent(UAnimSwitchComponent* InComp);

protected:
    virtual void NativeConstruct() override;

    // Optional BindWidget buttons: create a Widget Blueprint with these variable names to auto-bind.
    UPROPERTY(meta = (BindWidgetOptional)) UButton* AttackButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* EatButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* HitButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* TurnLeftButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* TurnRightButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* SitToggleButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* SwimToggleButton;
    UPROPERTY(meta = (BindWidgetOptional)) UButton* DieButton;

protected:
    UFUNCTION() void OnAttackClicked();
    UFUNCTION() void OnEatClicked();
    UFUNCTION() void OnHitClicked();
    UFUNCTION() void OnTurnLeftClicked();
    UFUNCTION() void OnTurnRightClicked();
    UFUNCTION() void OnSitToggleClicked();
    UFUNCTION() void OnSwimToggleClicked();
    UFUNCTION() void OnDieClicked();

private:
    UAnimSwitchComponent* ResolveComponent() const;

private:
    UPROPERTY(EditAnywhere, Category = "AnimSwitch|UI")
    TObjectPtr<AActor> TargetActor;

    UPROPERTY(EditAnywhere, Category = "AnimSwitch|UI")
    TObjectPtr<UAnimSwitchComponent> TargetComponent;
};

