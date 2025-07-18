// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PaddlePawn.h"
#include "PingPongGameMode.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

APaddlePawn::APaddlePawn()
{
    PrimaryActorTick.bCanEverTick = true;
    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(Root);
    PaddleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaddleMesh"));
    PaddleMesh->SetupAttachment(Root);
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(Root);
    SetReplicates(true);
    SetReplicatingMovement(true);
}

void APaddlePawn::BeginPlay()
{
    Super::BeginPlay();
    InitProperties();
}

void APaddlePawn::Server_OnGoalScored_Implementation(int32 TeamIndex)
{
    Multicast_Respawn();
}

void APaddlePawn::Multicast_Respawn_Implementation()
{
    SetActorLocation(StartLocation);
}

void APaddlePawn::OnGoalScoredHandler(int32 TeamIndex)
{
    if (HasAuthority())
    {
        Multicast_Respawn();
    }
    else
    {
        Server_OnGoalScored(TeamIndex);
    }
}

void APaddlePawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    MovePaddle(DeltaTime);
}

void APaddlePawn::MoveHorizontal(float InputValue)
{
    if (Controller && IsLocallyControlled())
    {
        CurrentInput = InputValue;
    }
}

void APaddlePawn::ServerMoveHorizontal_Implementation(float Value)
{
    CurrentInput = Value;
    MulticastMoveHorizontal(Value);
}

void APaddlePawn::MulticastMoveHorizontal_Implementation(float Value)
{
    if (!IsLocallyControlled())
    {
        CurrentInput = Value;
    }
    MoveHorizontal(Value);
}

void APaddlePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(APaddlePawn, CurrentInput);
}

void APaddlePawn::InitProperties()
{
    PingPongGameMode = Cast<APingPongGameMode>(UGameplayStatics::GetGameMode(this));
    if (PingPongGameMode)
    {
        PingPongGameMode->OnGoalScoredDelegate.AddUObject(this, &APaddlePawn::OnGoalScoredHandler);
    }

    StartLocation = GetActorLocation();
}

void APaddlePawn::MovePaddle(float DeltaTime)
{
    constexpr float MinX = -560.f;
    constexpr float MaxX = 560.f;

    FVector CurrentLocation = GetActorLocation();
    const FVector Direction = GetActorRightVector();
    FVector TargetLocation = CurrentLocation + Direction * CurrentInput * Speed * DeltaTime;
    TargetLocation.X = FMath::Clamp(TargetLocation.X, MinX, MaxX);
    CurrentLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, SmoothSpeed);
    SetActorLocation(CurrentLocation);
}