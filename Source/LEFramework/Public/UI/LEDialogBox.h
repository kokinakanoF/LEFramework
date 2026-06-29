//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UI/LEInOutWidget.h"
#include "LEDialogBox.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class UTextBlock;
class UPanelWidget;
class ULEToggleButton;
class ULERadioButtonManager;



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLEDialogBoxEvent, int32, InSelectedIndex);



//===========================================================================
//　クラス
//　@brief ダイアログボックス
//===========================================================================
UCLASS(Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULEDialogBox : public ULEInOutWidget
{
	GENERATED_BODY()
	
public:
	/**
	 * ダイアログを表示する
	 * 
	 * @param InTitle		タイトルテキスト
	 * @param InMessage		メッセージテキスト
	 * @param InButtonTexts	各ボタンテキスト
	 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void OpenDialog(const FText InTitle, const FText InMessage, const TArray<FText>& InButtonTexts);


private:
	/** いづれかのボタンが選択された */
	UFUNCTION()
	void OnAnyButtonSelected(const int32 InSelectedIndex);

	/** 
	 * ボタンを作成する
	 * @param InTexts 各ボタンテキスト
	 */
	void CreateButtons(const TArray<FText>& InTexts);


protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	//~End UUserWidget interface

	//~Begin ULEInOutWidget interface
	virtual void NativeFinishClose() override;
	//~End ULEInOutWidget interface




public:
	/** 項目選択時のイベント */
	UPROPERTY(BlueprintAssignable, Category = "LEDialogBox|Event")
	FOnLEDialogBoxEvent OnItemSelected;


protected:
	/** ボタンクラス */
	UPROPERTY(EditDefaultsOnly, Transient, Category = "LEDialogBox")
	TSubclassOf<ULEToggleButton> ButtonClass;

	/** 追加されるZOrder値 */
	UPROPERTY(EditDefaultsOnly, Transient, Category = "LEDialogBox")
	int32 AddedZOrder = 0;


	/** タイトルテキスト */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetOptional), Category = "LEDialogBox")
	TObjectPtr<UTextBlock> Txt_Title;

	/** メッセージテキスト */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetOptional), Category = "LEDialogBox")
	TObjectPtr<UTextBlock> Txt_Message;

	/** ボタンを格納するパネルウィジェット */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetOptional), Category = "LEDialogBox")
	TObjectPtr<UPanelWidget> Panel_Buttons;

	/** ボタンのマネージャ */
	UPROPERTY()
	TObjectPtr<ULERadioButtonManager> ButtonManager;
};
