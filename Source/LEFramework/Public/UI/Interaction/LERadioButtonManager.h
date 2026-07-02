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
	/** ラジオボタンの登録 */
	UFUNCTION(BlueprintCallable)
	void Register(ULERadioButton* InButton);

	/** ラジオボタンの登録解除 */
	UFUNCTION(BlueprintCallable)
	void Unregister(ULERadioButton* InButton);

	/** ラジオボタンの登録全解除 */
	UFUNCTION(BlueprintCallable)
	void UnregisterAll();


	UFUNCTION(BlueprintCallable)
	void ResetCheck();


	UFUNCTION(BlueprintCallable, BlueprintPure)
	ULERadioButton* GetCheckedButton() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetCheckedButtonIndex() const;


private:
	UFUNCTION()
	void HandleCheckedButtonChanged(ULEToggleButton* InButton);



public:
	UPROPERTY(BlueprintAssignable)
	FOnLERadioButtonManagerEvent OnCheckedButtonChanged;

protected:
	UPROPERTY()
	TArray<TObjectPtr<ULERadioButton>> RadioButtons;
};