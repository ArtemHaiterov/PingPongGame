// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameStateBase.h"
#include "Net/UnrealNetwork.h"

void APingPongGameStateBase::AddScore(int32 Team)
{
    if (HasAuthority())
    {
        if (Team == 0)
        {
            Team1Score++;
        }
        else
        {
            Team2Score++;
        }
    }
}

void APingPongGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(APingPongGameStateBase, Team1Score);
    DOREPLIFETIME(APingPongGameStateBase, Team2Score);
}

void APingPongGameStateBase::OnRep_ScoreUpdated()
{
}