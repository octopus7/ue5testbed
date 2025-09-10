#include "AnimSwitcherControlWidget.h"

#include "AnimSwitchComponent.h"
#include "Components/Button.h"
#include "Components/Widget.h"

void UAnimSwitcherControlWidget::SetTargetActor(AActor* InActor)
{
    TargetActor = InActor;
}

void UAnimSwitcherControlWidget::SetTargetComponent(UAnimSwitchComponent* InComp)
{
    TargetComponent = InComp;
}

void UAnimSwitcherControlWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (AttackButton)     { AttackButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnAttackClicked); }
    if (EatButton)        { EatButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnEatClicked); }
    if (HitButton)        { HitButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnHitClicked); }
    if (TurnLeftButton)   { TurnLeftButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnTurnLeftClicked); }
    if (TurnRightButton)  { TurnRightButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnTurnRightClicked); }
    if (SitToggleButton)  { SitToggleButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnSitToggleClicked); }
    if (SwimToggleButton) { SwimToggleButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnSwimToggleClicked); }
    if (DieButton)        { DieButton->OnClicked.AddDynamic(this, &UAnimSwitcherControlWidget::OnDieClicked); }
}

UAnimSwitchComponent* UAnimSwitcherControlWidget::ResolveComponent() const
{
    if (TargetComponent)
    {
        return TargetComponent;
    }
    if (TargetActor)
    {
        return TargetActor->FindComponentByClass<UAnimSwitchComponent>();
    }
    return nullptr;
}

void UAnimSwitcherControlWidget::OnAttackClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestAttack();
    }
}

void UAnimSwitcherControlWidget::OnEatClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestEat();
    }
}

void UAnimSwitcherControlWidget::OnHitClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestHit();
    }
}

void UAnimSwitcherControlWidget::OnTurnLeftClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestTurnLeft90();
    }
}

void UAnimSwitcherControlWidget::OnTurnRightClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestTurnRight90();
    }
}

void UAnimSwitcherControlWidget::OnSitToggleClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->SetSitting(!C->IsSitting());
    }
}

void UAnimSwitcherControlWidget::OnSwimToggleClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->SetSwimming(!C->IsSwimming());
    }
}

void UAnimSwitcherControlWidget::OnDieClicked()
{
    if (auto* C = ResolveComponent())
    {
        C->RequestDie();
    }
}

