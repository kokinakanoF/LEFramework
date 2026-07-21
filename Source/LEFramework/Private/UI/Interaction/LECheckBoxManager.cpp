//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LECheckBoxManager.h"
#include "UI/Interaction/LEToggleButton.h"



bool ULECheckBoxManager::Register(ULECheckBox* InCheckBox)
{
	if (InCheckBox && !CheckBoxes.Contains(InCheckBox))
	{
		InCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::HandleBoxStateChanged);
		CheckBoxes.Add(InCheckBox);

		// 登録成功
		return true;
	}

	// 登録失敗
	UE_LOG(LogTemp, Warning, TEXT("The box is invalid or already registered"));
	return false;
}

bool ULECheckBoxManager::Unregister(ULECheckBox* InCheckBox)
{
	if (CheckBoxes.Contains(InCheckBox))
	{
		InCheckBox->OnCheckStateChanged.RemoveDynamic(this, &ThisClass::HandleBoxStateChanged);
		CheckBoxes.Remove(InCheckBox);

		// 登録解除成功
		return true;
	}

	// 登録失敗
	UE_LOG(LogTemp, Warning, TEXT("Couldn't find specific CheckBox"));
	return false;
}



void ULECheckBoxManager::SetBoxCheckedFromIndex(int32 InIndex, bool bChecked)
{
	if (CheckBoxes.IsValidIndex(InIndex) && CheckBoxes[InIndex])
	{
		CheckBoxes[InIndex]->SetIsChecked(bChecked);
	}
}

void ULECheckBoxManager::SetAllBoxesChecked(bool bChecked)
{
	for (TObjectPtr<ULECheckBox> cb : CheckBoxes)
	{
		if (cb)
		{
			cb->SetIsChecked(bChecked);
		}
	}
}



ULECheckBox* ULECheckBoxManager::GetBoxFromIndex(int32 InIndex) const
{
	return CheckBoxes.IsValidIndex(InIndex) ? CheckBoxes[InIndex] : nullptr;
}

TArray<ULECheckBox*> ULECheckBoxManager::GetAllBoxesByState(bool bChecked) const
{
	TArray<ULECheckBox*> outArray;
	for (TObjectPtr<ULECheckBox> cb : CheckBoxes)
	{
		if (cb && cb->IsChecked() == bChecked)
		{
			outArray.Add(cb);
		}
	}

	return outArray;
}



bool ULECheckBoxManager::GetBoxStateFromIndex(int32 InIndex) const
{
	return CheckBoxes.IsValidIndex(InIndex) && CheckBoxes[InIndex]
		? CheckBoxes[InIndex]->IsChecked()
		: false;
}


TArray<bool> ULECheckBoxManager::GetAllBoxState() const
{
	TArray<bool> outArray;
	for (TObjectPtr<ULECheckBox> cb : CheckBoxes)
	{
		if (cb)
		{
			outArray.Add(cb->IsChecked());
		}
	}

	return outArray;
}


TArray<int32> ULECheckBoxManager::GetAllBoxIndexByState(bool bChecked)
{
	TArray<int32> outArray;
	for (int32 i = 0; i < CheckBoxes.Num(); i++)
	{
		TObjectPtr<ULECheckBox> cb = CheckBoxes[i];
		if (cb && cb->IsChecked() == bChecked)
		{
			outArray.Add(i);
		}
	}

	return outArray;
}



void ULECheckBoxManager::HandleBoxStateChanged(ULEToggleButton* InButton, bool bNewState)
{
	TObjectPtr<ULECheckBox> box = Cast<ULECheckBox>(InButton);

	int32 foundIndex = CheckBoxes.Find(box);
	if (foundIndex == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find specific CheckBox"));
	}
	else
	{
		// イベントの実行
		OnBoxStateChanged.Broadcast(box, foundIndex, bNewState);
	}
}