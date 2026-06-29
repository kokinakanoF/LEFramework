//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LEToggleButtonManager.h"
#include "UI/Interaction/LEToggleButton.h"


#if 0
void ULEToggleButtonManagerBase::RegisterButtonArray(const TArray<ULEToggleButton*>& InButtons)
{
	// 全ての登録を解除
	UnregisterAllButtons();

	// 新規登録
	for (ULEToggleButton* tb : InButtons)
	{
		RegisterButton(tb);
	}
}

void ULEToggleButtonManagerBase::RegisterButtonSingle(ULEToggleButton* InButton)
{
	// 管理下ではないボタンを登録する
	if (InButton && InButton->GetManager() != this)
	{
		RegisterButton(InButton);
	}
}



void ULEToggleButtonManagerBase::UnregisterAllButtons()
{
	for (int32 i = 0; i < Buttons.Num(); i++)
	{
		UnregisterButton(i);
	}

	Buttons.Empty();
}

void ULEToggleButtonManagerBase::UnregisterButtonSingle(ULEToggleButton* InButton)
{
	// 管理下のボタンのみ登録解除可能
	if (InButton && InButton->GetManager() == this)
	{
		UnregisterButtonFromIndex(InButton->GetButtonID());
	}
}

void ULEToggleButtonManagerBase::UnregisterButtonFromIndex(const int32 InIndex)
{
	if (Buttons.IsValidIndex(InIndex))
	{
		// 登録解除処理実行
		UnregisterButton(InIndex);

		// 配列から除去
		Buttons.RemoveAt(InIndex);

		// インデックスの更新のために再登録する
		TArray<ULEToggleButton*> registeredButtons = Buttons;
		Buttons.Empty();
		for (ULEToggleButton* tb : registeredButtons)
		{
			RegisterButton(tb);
		}
	}
}



void ULEToggleButtonManagerBase::ReceiveButtonStateChanged_Implementation(const int32 InButtonID, const bool bNewState)
{
	OnButtonStateChanged.Broadcast(InButtonID, bNewState);
}



void ULEToggleButtonManagerBase::SelectButton(const int32 InButtonID)
{
	if (ULEToggleButton* tb = GetButtonAt(InButtonID))
	{
		tb->SetIsChecked(true);
	}
}

void ULEToggleButtonManagerBase::ClearSelect()
{
	// オンのボタンを全てオフにする
	for (ULEToggleButton* tb : Buttons)
	{
		ForceTurnOffButton(tb);
	}
}



void ULEToggleButtonManagerBase::RegisterButton(ULEToggleButton* InButton)
{
	if (InButton)
	{
		// 末尾に登録するものとする
		InButton->RegisteredByManager(this, Buttons.Num());
	}

	// nullptrも登録可能
	Buttons.Add(InButton);
}

void ULEToggleButtonManagerBase::UnregisterButton(const int32 InIndex)
{
	if (ULEToggleButton* tb = GetButtonAt(InIndex))
	{
		// 登録解除した要素を無効にしておく
		tb->UnregisteredByManager();
		Buttons[InIndex] = nullptr;
	}
}



void ULEToggleButtonManagerBase::ForceTurnOffButton(ULEToggleButton* InButton)
{
	if (InButton)
	{
		// 一時的に必ずオフにできる状態にする
		const ELEToggleBehavior behavior = InButton->GetToggleBehavior();
		InButton->SetToggleBehavior(ELEToggleBehavior::ToggleButton);
		InButton->SetIsChecked(false);
		InButton->SetToggleBehavior(behavior);
	}
}
#endif