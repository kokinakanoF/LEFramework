//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UI/Interaction/LEButton.h"
#include "LEToggleButton.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class ULECheckBoxManager;
class ULERadioButtonManager;



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLEToggleButtonEvent, ULEToggleButton*, InButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLEToggleButtonStateEvent, ULEToggleButton*, InButton, bool, bNewState);



//===========================================================================
//　クラス
//　@brief 押すたびにオンとオフを切り替えるトグルボタン
//===========================================================================
UCLASS(ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULEToggleButton : public ULEButton
{
	GENERATED_BODY()
	
public:
	/** トグル状態の設定 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	virtual void SetIsChecked(bool bChecked);

	/** 状態をトグルする */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void ToggleChecked();

	/** @return オン状態であるか？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsChecked() const { return bIsChecked; }


protected:
	/** オンになる処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Checked"), Category = "LE Framework|UI")
	void BP_OnChecked();

	/** オフになる処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Unchecked"), Category = "LE Framework|UI")
	void BP_OnUnchecked();

protected:
	virtual void NativeOnChecked();
	virtual void NativeOnUnchecked();

	virtual void PlayCheckedAnim();
	virtual void PlayUncheckedAnim();


protected:
	//~Begin ULEButton interface
	virtual void NativeOnClicked() override;
	//~End ULEButton interface



public:
	/** オン時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEToggleButton|Event")
	FOnLEToggleButtonEvent OnChecked;

	/** オフ時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEToggleButton|Event")
	FOnLEToggleButtonEvent OnUnchecked;

	/** 状態変更時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEToggleButton|Event")
	FOnLEToggleButtonStateEvent OnCheckStateChanged;


protected:
	/** 
	 * オンになるアニメーション
	 * AN_Uncheckedがない場合は、非選択時にAN_Selectedを逆再生します
	 */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "LEToggleButton|Animation")
	TObjectPtr<UWidgetAnimation> AN_Checked;

	/** オンになるアニメーション */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional), Category = "LEToggleButton|Animation")
	TObjectPtr<UWidgetAnimation> AN_Unchecked;


private:
	bool bIsChecked = false;
};





//===========================================================================
//　クラス
//　@brief マネージャによって管理されるチェックボックス
//  @note  管理のためにサブクラスとして作成しています
//===========================================================================
UCLASS(ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULECheckBox: public ULEToggleButton
{
	GENERATED_BODY()
};





//===========================================================================
//　クラス
//　@brief マネージャによって管理されるラジオボタン
//		   自分でオンからオフになることは出来ません
//===========================================================================
UCLASS(ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULERadioButton : public ULEToggleButton
{
	GENERATED_BODY()

	friend class ULERadioButtonManager;

protected:
	//~Begin ULEToggleButton interface
	virtual void SetIsChecked(bool bChecked) override;
	//~End ULEToggleButton interface


private:
	/**
	 * 強制的にオフにします
	 * @note この関数はULERadioButtonManagerからのみ呼んでください
	 */
	void ForceUncheck();
};