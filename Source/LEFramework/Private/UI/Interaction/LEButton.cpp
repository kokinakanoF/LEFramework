//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LEButton.h"
// Slate
#include "Framework/Application/SlateApplication.h"
// Widget
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"



void ULEButton::SetIsPressed(bool bPressed)
{
	if (bIsPressable && bIsPressed != bPressed)
	{
		bIsPressed = bPressed;

		if (bPressed)
		{
			NativeOnPressed();
		}
		else
		{
			NativeOnReleased();
		}
	}
}


void ULEButton::NativeOnPressed()
{
	// アニメーション再生
	if (PressAnimPlayTimming == ELEButtonPressAnimPlayTimming::PressAndRelease)
	{
		PlayPressedAnim();
	}

	// SE再生
	FSlateApplication::Get().PlaySound(PressedSound);

	// イベント実行
	BP_OnPressed();
	OnPressed.Broadcast(this);
}


void ULEButton::NativeOnReleased()
{
	// アニメーション再生
	if (PressAnimPlayTimming == ELEButtonPressAnimPlayTimming::PressAndRelease)
	{
		PlayReleasedAnim();
	}

	// イベント実行
	BP_OnReleased();
	OnReleased.Broadcast(this);
}


void ULEButton::NativeOnClicked()
{
	// アニメーション再生
	if (PressAnimPlayTimming == ELEButtonPressAnimPlayTimming::Clicked)
	{
		PlayClickedAnim();
	}

	// SE再生
	FSlateApplication::Get().PlaySound(ClickedSound);

	// イベント実行
	BP_OnClicked();
	OnClicked.Broadcast(this);
}


void ULEButton::PlayPressedAnim()
{
	PlayAnimationForward(AN_Pressed);
}

void ULEButton::PlayReleasedAnim()
{
	PlayAnimationReverse(AN_Pressed);
}

void ULEButton::PlayClickedAnim()
{
	PlayAnimation(AN_Pressed);
}


EButtonClickMethod::Type ULEButton::GetClickMethodFromInputType(const FPointerEvent& InMouseEvent) const
{
	// タッチイベントの場合、ClickMethodに変換する
	if (InMouseEvent.IsTouchEvent())
	{
		switch (TouchMethod)
		{
		case EButtonTouchMethod::DownAndUp:
			return EButtonClickMethod::DownAndUp;
		case EButtonTouchMethod::Down:
			return EButtonClickMethod::MouseDown;
		case EButtonTouchMethod::PreciseTap:
			return EButtonClickMethod::PreciseClick;
		}
	}

	// マウスのクリックイベント
	return MouseClickMethod;
}

bool ULEButton::IsPreciseTapOrClick(const FPointerEvent& InMouseEvent) const
{
	return GetClickMethodFromInputType(InMouseEvent) == EButtonClickMethod::PreciseClick;
}


FReply ULEButton::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = FReply::Unhandled();
	if (GetIsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		// プレス処理
		SetIsPressed(true);

		// クリックイベントの処理タイミングはPressのため実行
		if (KeyPressMethod == EButtonPressMethod::ButtonPress)
		{
			// クリック処理
			NativeOnClicked();
			reply = FReply::Handled();
		}
		else
		{
			reply = FReply::Handled();
		}
	}
	else
	{
		reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	}

	return reply;
}

FReply ULEButton::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply reply = FReply::Unhandled();
	if (GetIsEnabled() && FSlateApplication::Get().GetNavigationActionFromKey(InKeyEvent) == EUINavigationAction::Accept)
	{
		const bool bWasPressed = bIsPressed;

		// リリース処理
		SetIsPressed(false);

		// クリックイベントの処理タイミングがRelease、またはプレス済みでDownAndUpなので実行
		if (KeyPressMethod == EButtonPressMethod::ButtonRelease || (KeyPressMethod == EButtonPressMethod::DownAndUp && bWasPressed))
		{
			// クリック処理
			NativeOnClicked();
			reply = FReply::Handled();
		}
		else
		{
			reply = FReply::Handled();
		}
	}
	else
	{
		reply = Super::NativeOnKeyUp(InGeometry, InKeyEvent);
	}

	return reply;
}

FReply ULEButton::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = FReply::Unhandled();
	if (GetIsEnabled())
	{
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || InMouseEvent.IsTouchEvent())
		{
			// プレス処理
			SetIsPressed(true);

			EButtonClickMethod::Type clickMethod = GetClickMethodFromInputType(InMouseEvent);
			if (clickMethod == EButtonClickMethod::MouseDown)
			{
				NativeOnClicked();
				reply = FReply::Handled();
			}
			else if (clickMethod == EButtonClickMethod::PreciseClick)
			{
				// キャプチャせずに入力を受け付けたことにする
				reply = FReply::Handled();
			}
			else
			{
				// マウスキャプチャをして、リリースに備える
				reply = FReply::Handled().CaptureMouse(GetCachedWidget().ToSharedRef());
			}
		}
	}

	if (!reply.IsEventHandled())
	{
		reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}

	return reply;
}

FReply ULEButton::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (reply.IsEventHandled())
	{
		return reply;
	}

	// ダブルクリックの処理が行われなければ、シングルクリックとして扱う
	return NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply ULEButton::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply reply = FReply::Unhandled();

	// 前提条件として、押されてある必要がある状態の確認
	const EButtonClickMethod::Type inputClickMethod = GetClickMethodFromInputType(InMouseEvent);
	const bool bMustBePressed = (inputClickMethod == EButtonClickMethod::DownAndUp) || (inputClickMethod == EButtonClickMethod::PreciseClick);
	
	const bool bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));
	if (bMeetsPressedRequirements)
	{
		if ((InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) || InMouseEvent.IsTouchEvent())
		{
			// リリース処理
			SetIsPressed(false);

			if (GetIsEnabled())
			{
				if (inputClickMethod == EButtonClickMethod::MouseDown)
				{
					// マウスキャプチャを行わない場合、OnMouseButtonUp()イベントを受け取れないため
					// OnMouseLeave()にてリリースを行う
				}
				else
				{
					bool bEventOverButton = IsHovered();
#if 0
					if (!bEventOverButton && InMouseEvent.IsTouchEvent())
					{
						bEventOverButton = InGeometry.IsUnderLocation(InMouseEvent.GetScreenSpacePosition());
					}
#endif
					if (bEventOverButton)
					{
						// ポインタが自身から離れていなければ処理を行う
						const bool bTriggerForTouchEvent = (inputClickMethod == EButtonClickMethod::PreciseClick);

						// MouseUpの場合は必ず処理を行う
						// DownAndUpの場合はキャプチャがあれば処理を行う
						const bool bTriggerForMouseEvent = (inputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

						if (bTriggerForTouchEvent || bTriggerForMouseEvent)
						{
							// クリック処理
							NativeOnClicked();
							reply = FReply::Handled();
						}
					}
				}
			}

			if (!reply.IsEventHandled())
			{
				reply = FReply::Handled();
			}
		}
	}

	if (!reply.GetMouseCaptor().IsValid() && HasMouseCapture())
	{
		reply.ReleaseMouseCapture();
	}

	return reply;
}


FReply ULEButton::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
#if 0
	if (IsPressed() && IsPreciseTapOrClick(InMouseEvent) && FSlateApplication::Get().HasTraveledFarEnoughToTriggerDrag(MouseEvent, PressedScreenSpacePosition))
	{
		SetIsPressed(false);
	}
#endif
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void ULEButton::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// マウスキャプチャを行わない場合、OnMouseButtonUp()イベントを受け取れないため
	// 状態のキャンセルのためにリリースを行う
	if (MouseClickMethod == EButtonClickMethod::MouseDown || IsPreciseTapOrClick(InMouseEvent))
	{
		SetIsPressed(false);
	}
}


void ULEButton::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);

	// リリース処理
	SetIsPressed(false);
}