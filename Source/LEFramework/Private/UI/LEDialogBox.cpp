//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LEDialogBox.h"
// Widget
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/Interaction/LEToggleButton.h"
#include "UI/Interaction/LERadioButtonManager.h"



void ULEDialogBox::OpenDialog(const FText InTitle, const FText InMessage, const TArray<FText>& InButtonTexts)
{
	// 入場
	AddToViewport(AddedZOrder);
	Open();

	// タイトルテキストの設定
	if (Txt_Title)
	{
		Txt_Title->SetText(InTitle);
	}

	// メッセージテキストの設定
	if (Txt_Message)
	{
		Txt_Message->SetText(InMessage);
	}

	// ボタンの作成
	CreateButtons(InButtonTexts);
}



void ULEDialogBox::OnAnyButtonSelected(const int32 InSelectedIndex)
{
	// 選択が行われたので閉じる
	Close();
}

void ULEDialogBox::CreateButtons(const TArray<FText>& InTexts)
{
	if (Panel_Buttons)
	{
		// リストのクリア
		Panel_Buttons->ClearChildren();

		// ボタンの作成
		TArray<ULEToggleButton*> buttons;
		for (const FText& txt : InTexts)
		{
			if (ULEToggleButton* tb = CreateWidget<ULEToggleButton>(this, ButtonClass))
			{
				tb->SetLabelText(txt);

				buttons.Add(tb);
				Panel_Buttons->AddChild(tb);
			}
		}
#if 0
		// ボタンの登録
		ButtonManager->RegisterButtonArray(buttons);
#endif
	}
}



void ULEDialogBox::NativeOnInitialized()
{
#if 0
	// ボタンマネージャの作成
	ButtonManager = NewObject<ULERadioButtonManager>(this);
	ButtonManager->OnSelectChanged.AddDynamic(this, &ThisClass::OnAnyButtonSelected);
#endif
	Super::NativeOnInitialized();
}

void ULEDialogBox::NativeFinishClose()
{
	Super::NativeFinishClose();
#if 0
	// イベントの実行
	OnItemSelected.Broadcast(ButtonManager->GetSelectedID());
#endif
	// 退場
	RemoveFromParent();
}