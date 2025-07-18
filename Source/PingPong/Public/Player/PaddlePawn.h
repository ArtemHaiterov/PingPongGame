// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PaddlePawn.generated.h"

class APingPongGameMode;
class UCameraComponent;

UCLASS()
class PINGPONG_API APaddlePawn : public APawn
{
    GENERATED_BODY()

public:
    APaddlePawn();

    virtual void Tick(float DeltaTime) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    UFUNCTION(Server, Unreliable)
    void ServerMoveHorizontal(float Value);
    UFUNCTION(NetMulticast, Unreliable)
    void MulticastMoveHorizontal(float Value);
    void MoveHorizontal(float InputValue);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* PaddleMesh;
    UPROPERTY(VisibleAnywhere)
    USceneComponent* Root;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;
    UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
    float Speed = 1400.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Speed", meta = (AllowPrivateAccess = "true"))
    float SmoothSpeed = 10.0f;
    UPROPERTY(Replicated)
    float CurrentInput = 0.0f;
    UPROPERTY()
    APingPongGameMode* PingPongGameMode;
    FVector StartLocation;
    UFUNCTION(Server, Reliable)
    void Server_OnGoalScored(int32 TeamIndex);
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Respawn();
    void OnGoalScoredHandler(int32 TeamIndex);
    void InitProperties();
    void MovePaddle(float DeltaTime);
};