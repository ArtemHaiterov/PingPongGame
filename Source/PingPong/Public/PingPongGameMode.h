// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PingPongGameMode.generated.h"

class APaddlePawn;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGoalScoredDelegate, int32);

UCLASS()
class PINGPONG_API APingPongGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    APingPongGameMode();
    virtual void PostLogin(APlayerController* NewPlayer) override;
    FOnGoalScoredDelegate OnGoalScoredDelegate;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category = "Spawn Class")
    TSubclassOf<class ABallActor> BallClass;
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<APaddlePawn> PaddleClass;
    UPROPERTY()
    AActor* CurrentBall;

private:
    int32 ConnectedPlayers = 0;
    TArray<APlayerController*> WaitingPlayers;
    UFUNCTION()
    void OnGoalScored(int32 ScoringTeamIndex);
    void StartGame();
    void NotifyWaitingPlayer(APlayerController* Player);
    void SpawnBall();
    void SpawnPlayers();
};