//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LEToggleButtonManager.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class ULEToggleButton;



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLEToggleButtonManagerEvent, int32, InButtonID, bool, bNewState);



//===========================================================================
//　クラス
//　@brief トグルボタン管理オブジェクト用インターフェース
//===========================================================================
UCLASS(Blueprintable, Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class ULEToggleButtonManagerBase : public UObject
{
	GENERATED_BODY()

#if 0
public:
	/** 
	 * ボタンの登録処理 
	 *
	 * @param InButtons	登録対象のボタン
	 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void RegisterButtonArray(const TArray<ULEToggleButton*>& InButtons);

	/** ボタンの単体登録処理 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void RegisterButtonSingle(ULEToggleButton* InButton);


	/** ボタンの登録解除処理 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void UnregisterAllButtons();

	/** ボタンの単体登録解除処理(ポインタ指定) */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void UnregisterButtonSingle(ULEToggleButton* InButton);

	/** ボタンの単体登録解除処理(インデックス指定) */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void UnregisterButtonFromIndex(const int32 InIndex);


	/** 
	 * 管理下のトグルボタンの状態が変わった
	 * 
	 * @param InButtonID	状態が変わったボタンのID
	 * @param bNewState		ボタンの新しい状態
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "LE Framework|UI")
	void ReceiveButtonStateChanged(const int32 InButtonID, const bool bNewState);
	

	/**
	 * ボタンを選択する
	 * 
	 * @param InButtonID 選択するボタンの番号
	 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void SelectButton(const int32 InButtonID);

	/** 選択の解除 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void ClearSelect();


	/** @return 管理対象のボタン(インデックス指定) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	ULEToggleButton* GetButtonAt(const int32 InButtonID) const { return Buttons.IsValidIndex(InButtonID) ? Buttons[InButtonID] : nullptr;; }

	/** @return 管理対象の全ボタン */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	TArray<ULEToggleButton*> GetAllButtons() const { return Buttons; }

	/** @return 登録されたボタン数 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	int32 GetButtonCount() const { return Buttons.Num(); }


protected:
	/** ボタンの登録処理本体 */
	virtual void RegisterButton(ULEToggleButton* InButton);

	/** ボタンの登録解除本体 */
	virtual void UnregisterButton(const int32 InIndex);


	/** 強制的にボタンをオフにする */
	void ForceTurnOffButton(ULEToggleButton* InButton);



public:
	/** 管理下のボタンの状態が変わった */
	UPROPERTY(BlueprintAssignable, Category = "LEToggleButton|Event")
	FOnLEToggleButtonManagerEvent OnButtonStateChanged;


protected:
	UPROPERTY()
	TArray<TObjectPtr<ULEToggleButton>> Buttons;
#endif
};



UCLASS()
class ULECheckBoxManager : public ULEToggleButtonManagerBase
{
	GENERATED_BODY()
};

UCLASS()
class ULERadioButtonManager : public ULEToggleButtonManagerBase
{
	GENERATED_BODY()
};