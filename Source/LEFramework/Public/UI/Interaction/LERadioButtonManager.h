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
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLEToggleButtonManagerEvent, int32, InButtonID, bool, bNewState);



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

protected:
	UPROPERTY()
	TArray<TObjectPtr<ULERadioButton>> RadioButtons;
};