// Fill out your copyright notice in the Description page of Project Settings.

#include "GameLogic/BallActor.h"

ABallActor::ABallActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    SetRootComponent(BallMesh);
    SetReplicates(true);
    SetReplicatingMovement(true);
    BallMesh->BodyInstance.bLockZTranslation = true;
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetNotifyRigidBodyCollision(true);
    BallMesh->SetCollisionProfileName(TEXT("BlockAll"));
    BallMesh->OnComponentHit.AddDynamic(this, &ABallActor::OnHit);
}

void ABallActor::BeginPlay()
{
    Super::BeginPlay();
    InitBallMovement();
}

void ABallActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!HasAuthority())
    {
        return;
    }

    ChangeBallTrajectory(Hit);
}

void ABallActor::ChangeBallTrajectory(const FHitResult& Hit)
{
    constexpr float BallSpeed = 60.f;
    const FVector HitNormal = Hit.Normal.GetSafeNormal();
    FVector NewDirection = FVector::VectorPlaneProject(Velocity, HitNormal).GetSafeNormal();
    const float RandomAngle = FMath::RandRange(-0.3f, 0.3f);
    NewDirection.X += RandomAngle;
    NewDirection = NewDirection.GetSafeNormal();
    Velocity = NewDirection * BallSpeed;
    BallMesh->SetPhysicsLinearVelocity(Velocity, true);
}

void ABallActor::InitBallMovement()
{
    Velocity = FVector(0.f, 300.f, 0.f);
    BallMesh->AddImpulse(FVector(0.f, 500.f, 0.f), FName("None"), true);
}