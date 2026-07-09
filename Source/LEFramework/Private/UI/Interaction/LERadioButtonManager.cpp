//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LERadioButtonManager.h"
#include "UI/Interaction/LEToggleButton.h"



bool ULERadioButtonManager::Register(ULERadioButton* InButton)
{
	if (InButton && !RadioButtons.Contains(InButton))
	{
		// イベントのバインド＆配列に追加
		InButton->OnChecked.AddDynamic(this, &ThisClass::ReceiveButtonChecked);
		RadioButtons.Add(InButton);

		// オン状態のボタンを登録する場合は、そのボタンが選択中であることにする
		if (InButton->IsChecked())
		{
			ReceiveButtonChecked(InButton);
		}

		// 登録成功
		return true;
	}

	// 登録失敗
	UE_LOG(LogTemp, Warning, TEXT("The button is invalid or already registered"));
	return false;
}

bool ULERadioButtonManager::Unregister(ULERadioButton* InButton)
{
	if (InButton)
	{
		const int32 foundIndex = RadioButtons.Find(InButton);
		if (foundIndex != INDEX_NONE)
		{
			// イベントのアンバインド
			InButton->OnChecked.RemoveDynamic(this, &ThisClass::ReceiveButtonChecked);
			RadioButtons.RemoveAt(foundIndex);

			// オン = 選択中のボタンだったので、選択ナシにする
			// 再利用を加味して、ボタンの状態はオンのままでもよいものとする
			if (foundIndex == CheckedButtonIndex)
			{
				UpdateCheckedButtonIndex(INDEX_NONE);
			}

			// 登録解除成功
			return true;
		}
	}

	// 登録解除失敗
	UE_LOG(LogTemp, Warning, TEXT("Couldn't find specific RadioButton"));
	return false;
}

void ULERadioButtonManager::UnregisterAll()
{
	for (ULERadioButton* rb : RadioButtons)
	{
		if (rb)
		{
			// イベントのアンバインド
			rb->OnChecked.RemoveDynamic(this, &ThisClass::ReceiveButtonChecked);
		}
	}

	RadioButtons.Empty();
	UpdateCheckedButtonIndex(INDEX_NONE);
}


void ULERadioButtonManager::CheckButtonFromIndex(int32 InIndex)
{
	if (TObjectPtr<ULERadioButton> rb = GetButtonFromIndex(InIndex))
	{
		rb->SetIsChecked(true);
	}
}

void ULERadioButtonManager::ResetCheck()
{
	if (TObjectPtr<ULERadioButton> rb = GetCheckedButton())
	{
		// 選択解除
		rb->ForceUncheck();

		// イベントの実行
		UpdateCheckedButtonIndex(INDEX_NONE);
	}
}


ULERadioButton* ULERadioButtonManager::GetCheckedButton() const
{
	return GetButtonFromIndex(CheckedButtonIndex);
}

ULERadioButton* ULERadioButtonManager::GetButtonFromIndex(int32 InIndex) const
{
	return RadioButtons.IsValidIndex(InIndex) ? RadioButtons[InIndex] : nullptr;
}



void ULERadioButtonManager::UpdateCheckedButtonIndex(int32 InIndex)
{
	CheckedButtonIndex = InIndex;
	OnCheckedButtonChanged.Broadcast(GetCheckedButton(), CheckedButtonIndex);
}


void ULERadioButtonManager::ReceiveButtonChecked(ULEToggleButton* InButton)
{
	TObjectPtr<ULERadioButton> rb = Cast<ULERadioButton>(InButton);

	int32 newIndex = RadioButtons.Find(rb);
	if (!InButton || newIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find specific RadioButton"));
	}
	else
	{
		// 現在選択中のボタンをオフにする
		if (ULERadioButton* lastChecked = GetCheckedButton())
		{
			lastChecked->ForceUncheck();
		}

		// イベントの実行
		UpdateCheckedButtonIndex(CheckedButtonIndex);
	}
}