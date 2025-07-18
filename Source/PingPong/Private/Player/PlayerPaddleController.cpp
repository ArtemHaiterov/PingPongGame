#include "Player/PlayerPaddleController.h"
#include "Player/PaddlePawn.h"
#include "Widgets/ScoreWidget.h"

void APlayerPaddleController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveRight", this, &APlayerPaddleController::MoveRight);
}

void APlayerPaddleController::BeginPlayingState()
{
	Super::BeginPlayingState();
	CreateAndShowScoreWidget();
	CachedPaddle = Cast<APaddlePawn>(GetPawn());
}

void APlayerPaddleController::MoveRight(float Value)
{
	if (CachedPaddle)
	{
		CachedPaddle->ServerMoveHorizontal(Value);
	}
	else
	{
		CachedPaddle = Cast<APaddlePawn>(GetPawn());
		if (CachedPaddle)
		{
			CachedPaddle->ServerMoveHorizontal(Value);
		}
	}
}

void APlayerPaddleController::CreateAndShowScoreWidget()
{
	if (IsLocalController() && !ScoreWidgetInstance && ScoreWidgetClass)
	{
		ScoreWidgetInstance = CreateWidget<UScoreWidget>(this, ScoreWidgetClass);
		if (ScoreWidgetInstance)
		{
			ScoreWidgetInstance->AddToViewport();
		}
	}
}
