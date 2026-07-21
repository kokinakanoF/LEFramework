//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Interaction/LESelectableWidget.h"
// Widget
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
// BFL
#include "Blueprint/WidgetBlueprintLibrary.h"

#include "Kismet/GameplayStatics.h"



ULESelectableWidget::ULESelectableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::Visible);
}


void ULESelectableWidget::SetIsSelectable(bool bEnable)
{
	if (bIsSelectable != bEnable)
	{
		bIsSelectable = bEnable;

		// 選択状態で選択不可にする場合は、選択解除
		if (bIsSelected && !bEnable)
		{
			SetIsSelected(false);
		}
	}	
}


void ULESelectableWidget::SetLabelText(const FText& InText)
{
	LabelText = InText;
	RefreshLabelTextBlock();
}


void ULESelectableWidget::SetIsSelected(bool bSelected)
{
	if (bIsSelectable && bIsSelected != bSelected)
	{
		bIsSelected = bSelected;

		if (bSelected)
		{
			NativeOnSelected();
		}
		else
		{
			NativeOnDeselected();
		}
	}
}



void ULESelectableWidget::NativeOnSelected()
{
	// アニメーション＆SE再生
	PlaySelectedAnimation();
	FSlateApplication::Get().PlaySound(SelectedSound);

	// イベントの実行
	BP_OnSelected();
	OnSelected.Broadcast(this);
}

void ULESelectableWidget::NativeOnDeselected()
{
	// アニメーション＆SE再生
	PlayDeselectedAnimation();
	FSlateApplication::Get().PlaySound(DeselectedSound);

	// イベントの実行
	BP_OnDeselected();
	OnDeselected.Broadcast(this);
}



void ULESelectableWidget::PlaySelectedAnimation()
{
	if (AN_Deselected)
	{
		StopAnimation(AN_Deselected);
	}

	PlayAnimationForward(AN_Selected);
}

void ULESelectableWidget::PlayDeselectedAnimation()
{
	if (AN_Deselected)
	{
		// AN_Deselectedがある場合はそちらを使う
		StopAnimation(AN_Selected);
		PlayAnimationForward(AN_Deselected);
	}
	else
	{
		PlayAnimationReverse(AN_Selected);
	}
}


void ULESelectableWidget::RefreshLabelTextBlock()
{
	// 型に応じて設定方法を変える
	if (UTextBlock* tb = Cast<UTextBlock>(Txt_Label))
	{
		tb->SetText(LabelText);
	}
	else if (URichTextBlock* rtb = Cast<URichTextBlock>(Txt_Label))
	{
		rtb->SetText(LabelText);
	}
}



void ULESelectableWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	// ラベルテキストの更新
	RefreshLabelTextBlock();
}


void ULESelectableWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (GetIsEnabled() && EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Mouse))
	{
		// 選択
		SetIsSelected(true);

		// フォーカスも有効ならフォーカスを与える
		if (EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Focus))
		{
			APlayerController* controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), InMouseEvent.GetUserIndex());
			SetUserFocus(controller);
		}
	}
}

FReply ULESelectableWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (GetIsEnabled() && EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Mouse))
	{
		// マウスを表示した瞬間はMouseEnterを認識できないため、ここでチェックする
		// ただし連続して呼ばれるのでSetIsSelected()自体の呼び出し回数を減らす　
		if (!bIsSelected && IsHovered())
		{
			SetIsSelected(true);
			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void ULESelectableWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	
	if (GetIsEnabled() && EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Mouse))
	{
		// フォーカスを持っている場合は選択を解除しない
		if (EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Focus))
		{
			APlayerController* controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), InMouseEvent.GetUserIndex());
			if (HasUserFocus(controller) || HasUserFocusedDescendants(controller))
			{
				return;
			}
		}

		// 選択解除を行う
		SetIsSelected(false);
	}
}


void ULESelectableWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	// 選択
	if (GetIsEnabled() && EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Focus))
	{
		SetIsSelected(true);
	}
}

void ULESelectableWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	
	if (GetIsEnabled() && EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Focus))
	{
		// マウスホバーしている場合は選択状態のままにしておく
		if (EnumHasAnyFlags((ELESelectMethod)SelectMethod, ELESelectMethod::Mouse) && IsHovered())
		{
			return;
		}

		// 選択解除
		SetIsSelected(false);
	}
}