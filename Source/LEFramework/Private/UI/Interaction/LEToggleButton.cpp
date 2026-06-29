//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LEToggleButton.h"
#include "UI/Interaction/LEToggleButtonManager.h"



void ULEToggleButton::SetIsChecked(const bool bNewState)
{
	RequestNewToggleState(bNewState);
}

void ULEToggleButton::ToggleChecked()
{
	RequestNewToggleState(!bIsChecked);
}


void ULEToggleButton::RequestNewToggleState(const bool bNewState)
{
	if (bIsChecked != bNewState)
	{
		if (bNewState)
		{
			NativeOnChecked();
		}
		else
		{
			NativeOnUnchecked();
		}
	}
}

void ULEToggleButton::NativeOnChecked()
{
	bIsChecked = true;

	// アニメーション再生
	PlayCheckedAnim();

	// イベントの実行
	BP_OnChecked();
	OnChecked.Broadcast(this);
	OnToggleChecked.Broadcast(this, true);
}

void ULEToggleButton::NativeOnUnchecked()
{
	bIsChecked = false;

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




void ULECheckBox::Registered(TObjectPtr<ULECheckBoxManager> InManager, int32 InManageID)
{
	Manager = InManager;
	ManagedID = InManageID;
}

void ULECheckBox::Unreagistered()
{
	Manager = nullptr;
	ManagedID = -1;
}


void ULECheckBox::NativeOnChecked()
{
	Super::NativeOnChecked();

	if (Manager)
	{

	}
}

void ULECheckBox::NativeOnUnchecked()
{
	Super::NativeOnUnchecked();

	if (Manager)
	{

	}
}



void ULERadioButton::Registered(TObjectPtr<ULERadioButtonManager> InManager, int32 InManageID)
{
	Manager = InManager;
	ManagedID = InManageID;
}

void ULERadioButton::Unreagistered()
{
	Manager = nullptr;
	ManagedID = -1;
}


void ULERadioButton::NativeOnChecked()
{
	Super::NativeOnChecked();

	if (Manager)
	{

	}
}

void ULERadioButton::NativeOnUnchecked()
{
	Super::NativeOnUnchecked();

	if (Manager)
	{

	}
}