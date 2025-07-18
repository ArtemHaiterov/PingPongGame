// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PingPongGameStateBase.generated.h"

UCLASS()
class PINGPONG_API APingPongGameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetTeam1Score() const { return Team1Score; }

    UFUNCTION(BlueprintCallable, Category = "Score")
    int32 GetTeam2Score() const { return Team2Score; }

    void AddScore(int32 Team);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    UFUNCTION()
    void OnRep_ScoreUpdated();

private:
    UPROPERTY(ReplicatedUsing = OnRep_ScoreUpdated)
    int32 Team1Score = 0;
    UPROPERTY(ReplicatedUsing = OnRep_ScoreUpdated)
    int32 Team2Score = 0;
};