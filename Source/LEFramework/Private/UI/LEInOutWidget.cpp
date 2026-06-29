//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LEInOutWidget.h"



void ULEInOutWidget::Open()
{
	if (IsEnableOpen())
	{
		NativeStartOpen();
	}
}

void ULEInOutWidget::Close()
{
	if (IsEnableClose())
	{
		NativeStartClose();
	}
}

void ULEInOutWidget::ToggleInOut()
{
	if (IsClosed())
	{
		Open();
	}
	else
	{
		Close();
	}
}


bool ULEInOutWidget::IsEnableOpen()
{
	if (!IsInViewport())
	{
		return false;
	}

	switch (OverrideSetting)
	{
	case ELEInOutOverrideSetting::Always:
		return true;
	case ELEInOutOverrideSetting::ToggleOnly:
		return IsClosed();
	case ELEInOutOverrideSetting::AnimCompleted:
		return IsCloseFinished();
	}

	return false;
}

bool ULEInOutWidget::IsEnableClose()
{
	if (!IsInViewport())
	{
		return false;
	}

	switch (OverrideSetting)
	{
	case ELEInOutOverrideSetting::Always:
		return true;
	case ELEInOutOverrideSetting::ToggleOnly:
		return IsOpened();
	case ELEInOutOverrideSetting::AnimCompleted:
		return IsOpenFinished();
	}

	return false;
}



void ULEInOutWidget::NativeStartOpen()
{
	IOState = ELEInOutState::Opening;

	// アニメーション再生
	PlayOpenAnim();

	// SE再生
	FSlateApplication::Get().PlaySound(OpenSound);

	// イベントの実行
	StartOpen();
	OnStartOpen.Broadcast();
}

void ULEInOutWidget::NativeFinishOpen()
{
	// イベントの実行
	FinishOpen();
	OnFinishOpen.Broadcast();
}

void ULEInOutWidget::NativeStartClose()
{
	IOState = ELEInOutState::Closing;

	// アニメーション再生
	PlayCloseAnim();

	// SE再生
	FSlateApplication::Get().PlaySound(CloseSound);

	// イベントの実行
	StartClose();
	OnStartClose.Broadcast();
}

void ULEInOutWidget::NativeFinishClose()
{
	// イベントの実行
	FinishClose();
	OnFinishClose.Broadcast();

	// 自動でDestructする
	if (bAutoDestruct)
	{
		RemoveFromParent();
	}
}



void ULEInOutWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 初期状態の設定
	IOState = bDefaultOpen ? ELEInOutState::OpenFinished : ELEInOutState::CloseFinished;
}

void ULEInOutWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 自動でオープンする
	if (bAutoOpen)
	{
		Open();
	}
}

void ULEInOutWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	Super::OnAnimationFinished_Implementation(Animation);

	// 入場時処理
	if (IsOpening())
	{
		if (Animation == AN_Open)
		{
			IOState = ELEInOutState::OpenFinished;
			NativeFinishOpen();
			return;
		}
	}

	// 退場時処理
	if (IsClosing())
	{
		UWidgetAnimation* closeAnim = AN_Close ? AN_Close : AN_Open;
		if (Animation == closeAnim)
		{
			IOState = ELEInOutState::CloseFinished;
			NativeFinishClose();
			return;
		}
	}
}



void ULEInOutWidget::PlayOpenAnim()
{
	StopAnimation(AN_Close);
	PlayAnimationForward(AN_Open);
}

void ULEInOutWidget::PlayCloseAnim()
{
	if (AN_Close)
	{
		StopAnimation(AN_Open);
		PlayAnimation(AN_Close);
	}
	else
	{
		PlayAnimationReverse(AN_Open);
	}
}