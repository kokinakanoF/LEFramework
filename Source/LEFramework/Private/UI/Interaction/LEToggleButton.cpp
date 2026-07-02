//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LEToggleButton.h"
#include "UI/Interaction/LECheckBoxManager.h"
#include "UI/Interaction/LERadioButtonManager.h"



void ULEToggleButton::SetIsChecked(bool bChecked)
{
	if (bIsChecked != bChecked)
	{
		bIsChecked = bChecked;

		if (bChecked)
		{
			NativeOnChecked();
		}
		else
		{
			NativeOnUnchecked();
		}
	}
}

void ULEToggleButton::ToggleChecked()
{
	SetIsChecked(!bIsChecked);
}



void ULEToggleButton::NativeOnChecked()
{
	// アニメーション再生
	PlayCheckedAnim();

	// イベントの実行
	BP_OnChecked();
	OnChecked.Broadcast(this);
	OnToggleChecked.Broadcast(this, true);
}

void ULEToggleButton::NativeOnUnchecked()
{
	// アニメーション再生
	PlayUncheckedAnim();

	// イベントの実行
	BP_OnUnchecked();
	OnUnchecked.Broadcast(this);
	OnToggleChecked.Broadcast(this, false);
}


void ULEToggleButton::PlayCheckedAnim()
{
	if (AN_Unchecked)
	{
		StopAnimation(AN_Unchecked);
	}

	PlayAnimationForward(AN_Checked);
}

void ULEToggleButton::PlayUncheckedAnim()
{
	if (AN_Unchecked)
	{
		StopAnimation(AN_Checked);
		PlayAnimation(AN_Unchecked);
	}
	else
	{
		PlayAnimationReverse(AN_Checked);
	}
}


void ULEToggleButton::NativeOnClicked()
{
	Super::NativeOnClicked();

	// 状態を反転させる
	ToggleChecked();
}



void ULERadioButton::SetIsChecked(bool bChecked)
{
	// オン→オフにはなれない
	if (!IsChecked())
	{
		Super::SetIsChecked(bChecked);
	}
}


void ULERadioButton::ForceUncheck()
{
	if (IsChecked())
	{
		Super::SetIsChecked(false);
	}
}