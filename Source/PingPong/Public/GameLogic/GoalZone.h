// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalZone.generated.h"

class UBoxComponent;

UCLASS()
class PINGPONG_API AGoalZone : public AActor
{
    GENERATED_BODY()

public:
    AGoalZone();

protected:
    UPROPERTY(VisibleAnywhere)
    UBoxComponent* TriggerBox;
    UPROPERTY(EditAnywhere)
    int32 TeamIndex = 0;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
};