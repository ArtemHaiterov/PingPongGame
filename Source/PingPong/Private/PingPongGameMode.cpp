// Fill out your copyright notice in the Description page of Project Settings.


#include "PingPongGameMode.h"
#include "GameLogic/BallActor.h"
#include "Player/PaddlePawn.h"
#include "PingPongGameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

APingPongGameMode::APingPongGameMode()
{
    DefaultPawnClass = nullptr;
    GameStateClass = APingPongGameStateBase::StaticClass();
}

void APingPongGameMode::BeginPlay()
{
    Super::BeginPlay();
    OnGoalScoredDelegate.AddUObject(this, &APingPongGameMode::OnGoalScored);
}

void APingPongGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ConnectedPlayers++;
    WaitingPlayers.Add(NewPlayer);

    constexpr int32 NumberOfPlayers = 2;
    if (ConnectedPlayers < NumberOfPlayers)
    {
        NotifyWaitingPlayer(NewPlayer);
    }
    else if (ConnectedPlayers == NumberOfPlayers)
    {
        StartGame();
    }
}

void APingPongGameMode::OnGoalScored(int32 ScoringTeamIndex)
{
    APingPongGameStateBase* MyState = GetGameState<APingPongGameStateBase>();
    if (MyState)
    {
        MyState->AddScore(ScoringTeamIndex == 0);
    }

    SpawnBall();
}

void APingPongGameMode::NotifyWaitingPlayer(APlayerController* Player)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Waiting for second player..."));
    }
}

void APingPongGameMode::StartGame()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Match Started!"));
    }

    SpawnPlayers();
    SpawnBall();
}

void APingPongGameMode::SpawnPlayers()
{
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

    if (PlayerStarts.Num() < WaitingPlayers.Num())
    {
        return;
    }

    for (int32 i = 0; i < WaitingPlayers.Num(); ++i)
    {
        APlayerController* PC = WaitingPlayers[i];
        const AActor* StartPoint = PlayerStarts[i];

        FVector SpawnLocation = StartPoint->GetActorLocation();
        FRotator SpawnRotation = StartPoint->GetActorRotation();

        APaddlePawn* NewPaddle = GetWorld()->SpawnActor<APaddlePawn>(PaddleClass, SpawnLocation, SpawnRotation);
        if (NewPaddle)
        {
            PC->Possess(NewPaddle);
        }
    }
}

void APingPongGameMode::SpawnBall()
{
    if (!BallClass)
        return;

    FVector Location = FVector(0.f, 0.f, -140.f);
    FRotator Rotation = FRotator::ZeroRotator;

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    CurrentBall = GetWorld()->SpawnActor<AActor>(BallClass, Location, Rotation, Params);
}