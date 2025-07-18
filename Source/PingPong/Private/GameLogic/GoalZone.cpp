// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLogic/GoalZone.h"
#include "Components/BoxComponent.h"
#include "GameLogic/BallActor.h"
#include "Kismet/GameplayStatics.h"
#include "PingPongGameMode.h"

AGoalZone::AGoalZone()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    SetRootComponent(TriggerBox);
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGoalZone::OnOverlapBegin);
    SetReplicates(true);
}

void AGoalZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (!HasAuthority())
    {
        return;
    }

    ABallActor* Ball = Cast<ABallActor>(OtherActor);
    if (Ball)
    {
        const APingPongGameMode* GameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(this));
        if (GameMode)
        {
            GameMode->OnGoalScoredDelegate.Broadcast(TeamIndex);
        }

        Ball->Destroy();
    }
}