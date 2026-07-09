//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LERadioButtonManager.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class ULERadioButton;



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLERadioButtonManagerEvent, ULERadioButton*, InButton, int32, InIndex);



//===========================================================================
//　クラス
//　@brief トグルボタン管理クラス
// 
//  @note 作成中
//===========================================================================
UCLASS(BlueprintType)
class ULERadioButtonManager : public UObject
{
	GENERATED_BODY()

public:
	/** 
	 * ラジオボタンの登録
	 * @note オン状態のボタンを登録した場合、選択はそのボタンに移ります
	 */
	UFUNCTION(BlueprintCallable)
	bool Register(ULERadioButton* InButton);

	/** 
	 * ラジオボタンの登録解除
	 * @note オン状態のボタンの登録を解除した場合、選択も解除されます
	 */
	UFUNCTION(BlueprintCallable)
	bool Unregister(ULERadioButton* InButton);

	/** ラジオボタンの登録全解除 */
	UFUNCTION(BlueprintCallable)
	void UnregisterAll();


	/** 選択したボタンをオン状態にします */
	UFUNCTION(BlueprintCallable)
	void CheckButtonFromIndex(int32 InIndex);

	/** 選択の解除 */
	UFUNCTION(BlueprintCallable)
	void ResetCheck();


	/** @return 選択中のボタン */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULERadioButton* GetCheckedButton() const;

	/** @return 選択中のボタンのインデックス */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCheckedButtonIndex() const { return CheckedButtonIndex; }

	/** @return 登録されている全ボタン */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<ULERadioButton*> GetAllButtons() const { return RadioButtons; }

	/** @return InIndexで指定したインデックスのボタン */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULERadioButton* GetButtonFromIndex(int32 InIndex) const;


private:
	/** 選択中のボタンのインデックスを更新します */
	void UpdateCheckedButtonIndex(int32 InIndex);

	/** 
	 * ボタンがオン状態になった際に呼ばれる処理
	 * 現在選択中のボタンをオフにして、イベントを実行します
	 */
	UFUNCTION()
	void ReceiveButtonChecked(ULEToggleButton* InButton);



public:
	/** 選択中のボタンが切り替わった際に呼ばれるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnLERadioButtonManagerEvent OnCheckedButtonChanged;

protected:
	UPROPERTY()
	TArray<TObjectPtr<ULERadioButton>> RadioButtons;

	int32 CheckedButtonIndex = INDEX_NONE;
};