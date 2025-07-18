// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerPaddleController.generated.h"

class UScoreWidget;
class APaddlePawn;

UCLASS()
class PINGPONG_API APlayerPaddleController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void SetupInputComponent() override;
    virtual void BeginPlayingState() override;
    void MoveRight(float Value);
    void CreateAndShowScoreWidget();

private:
    UPROPERTY()
    APaddlePawn* CachedPaddle = nullptr;
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UScoreWidget> ScoreWidgetClass;
    UPROPERTY()
    UScoreWidget* ScoreWidgetInstance;
};