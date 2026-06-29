//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LEUserWidget.h"
#include "Animation/UMGSequencePlayer.h"



void ULEUserWidget::ActivateWidget()
{
	if (!IsActivated())
	{
		NativeOnActivated();
	}
}

void ULEUserWidget::DeactivateWidget()
{
	if (IsActivated())
	{
		NativeOnDeactivated();
	}
}


void ULEUserWidget::SetWidgetPaused_Implementation(const bool bPaused)
{
	if (bEnablePause)
	{
		bIsPaused = bPaused;
		SetPauseAnimations(bPaused);
	}
}


void ULEUserWidget::NativeOnActivated()
{
	bIsActive = true;

	// 可視性の設定
	if (bSetVisibilityOnActivated)
	{
		SetVisibility(ActivatedVisibility);
	}

	// イベントの実行
	BP_OnActivated();
	OnActivated.Broadcast();
}

void ULEUserWidget::NativeOnDeactivated()
{
	bIsActive = false;

	// 可視性の設定
	if (bSetVisibilityOnDeactivated)
	{
		SetVisibility(DeactivatedVisibility);
	}
		
	// イベントの実行
	BP_OnDeactivated();
	OnDeactivated.Broadcast();
}

FReply ULEUserWidget::NativeOnAnyButtonDown(const FGeometry& InGeometry, const FKey& InKey)
{
	return OnAnyButtonDown(InGeometry, InKey).NativeReply;
}
FReply ULEUserWidget::NativeOnAnyButtonUp(const FGeometry& InGeometry, const FKey& InKey)
{
	return OnAnyButtonUp(InGeometry, InKey).NativeReply;
}


void ULEUserWidget::NativePausedTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	PausedTick(MyGeometry, InDeltaTime);
}


void ULEUserWidget::SetPauseAnimations(const bool bPause)
{
	const EMovieScenePlayerStatus::Type status = bPause ? EMovieScenePlayerStatus::Paused : EMovieScenePlayerStatus::Playing;
	for (UUMGSequencePlayer* player : ActiveSequencePlayers)
	{
		player->SetPlaybackStatus(status);
	}
}



void ULEUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (bAutoActivate)
	{
		ActivateWidget();
	}
}

void ULEUserWidget::NativeDestruct()
{
	Super::NativeConstruct();

	DeactivateWidget();
}

void ULEUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsPaused)
	{
		NativePausedTick(MyGeometry, InDeltaTime);
	}
}

FReply ULEUserWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = NativeOnAnyButtonDown(InGeometry, InMouseEvent.GetEffectingButton());

	if (!reply.IsEventHandled())
	{
		reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	return reply;
}

FReply ULEUserWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = NativeOnAnyButtonUp(InGeometry, InMouseEvent.GetEffectingButton());

	if (!reply.IsEventHandled())
	{
		reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	}

	return reply;
}

FReply ULEUserWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = NativeOnAnyButtonDown(InGeometry, InKeyEvent.GetKey());

	if (!reply.IsEventHandled())
	{
		reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	}

	return reply;
}

FReply ULEUserWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = NativeOnAnyButtonUp(InGeometry, InKeyEvent.GetKey());

	if (!reply.IsEventHandled())
	{
		reply = Super::NativeOnKeyUp(InGeometry, InKeyEvent);
	}

	return reply;
}
