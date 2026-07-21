//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "LECheckBoxManager.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class ULECheckBox;



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnLECheckBoxManagerEvent, ULECheckBox*, InBox, int32, InIndex, bool, bNewState);



//===========================================================================
//　クラス
//　@brief トグルボタン管理クラス
// 
//  @note 作成中
//===========================================================================
UCLASS(BlueprintType)
class ULECheckBoxManager : public UObject
{
	GENERATED_BODY()

	friend class ULECheckBox;

public:
	/** チェックボックスの登録処理 */
	UFUNCTION(BlueprintCallable)
	bool Register(ULECheckBox* InCheckBox);

	/** チェックボックスの登録解除処理 */
	UFUNCTION(BlueprintCallable)
	bool Unregister(ULECheckBox* InCheckBox);


	/** 
	 * 指定したボタンの状態を変えます 
	 * 
	 * @param InIndex- 変更対象のボタンのインデックス
	 * @param bChecked - 変更後の状態
	 */
	UFUNCTION(BlueprintCallable)
	void SetBoxCheckedFromIndex(int32 InIndex, bool bChecked);

	/** 管理下の全ボタンを指定の状態にします */
	UFUNCTION(BlueprintCallable)
	void SetAllBoxesChecked(bool bChecked);


	/** @return InIndexで指定したインデックスのボタン */
	UFUNCTION(BlueprintCallable)
	ULECheckBox* GetBoxFromIndex(int32 InIndex) const;

	/** @return bCheckedと同じ状態の全チェックボックス */
	UFUNCTION(BlueprintCallable)
	TArray<ULECheckBox*> GetAllBoxesByState(bool bChecked) const;

	/** @return 全チェックボックス */
	UFUNCTION(BlueprintCallable)
	TArray<ULECheckBox*> GetAllBoxes() const { return CheckBoxes; }


	/** @return 指定したインデックスのチェックボックスの状態 */
	UFUNCTION(BlueprintCallable)
	bool GetBoxStateFromIndex(int32 InIndex) const;

	/** @return 全チェックボックスの状態 */
	UFUNCTION(BlueprintCallable)
	TArray<bool> GetAllBoxState() const;


	/** @return 指定した状態のボタンのインデックス */
	UFUNCTION(BlueprintCallable)
	TArray<int32> GetAllBoxIndexByState(bool bChecked);


private:
	UFUNCTION()
	void HandleBoxStateChanged(ULEToggleButton* InButton, bool bNewState);



public:
	UPROPERTY(BlueprintAssignable)
	FOnLECheckBoxManagerEvent OnBoxStateChanged;

protected:
	UPROPERTY()
	TArray<TObjectPtr<ULECheckBox>> CheckBoxes;
};