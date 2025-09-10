#include "AnimSwitchCharacter.h"

#include "AnimSwitchComponent.h"

AAnimSwitchCharacter::AAnimSwitchCharacter()
{
    PrimaryActorTick.bCanEverTick = false; // no behavior yet

    AnimSwitchComponent = CreateDefaultSubobject<UAnimSwitchComponent>(TEXT("AnimSwitchComponent"));
}

void AAnimSwitchCharacter::BeginPlay()
{
    Super::BeginPlay();
    // Structure-only: no behavior here yet
}

