//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "LESeparatedCounterTextBlock.generated.h"



//===========================================================================
//　クラス
//　@brief カウンターテキスト表示用テキストブロック
//===========================================================================
UCLASS()
class ULESeparatedCounterTextBlock : public URichTextBlock
{
	GENERATED_BODY()
	
public:
	ULESeparatedCounterTextBlock(const FObjectInitializer& ObjectInitializer);

	/** 値の設定 */
	UFUNCTION(BlueprintCallable)
	virtual void SetCounterValue(int32 InValue);

protected:
	//~Begin UUserWidget interface
	virtual void SynchronizeProperties() override;
	//~End UUserWidget interface


protected:
	/** フォーマット用テキスト */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText TextFormat;

	/** 最大表示ケタ数 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDigitNum = 5;

	/** 最大桁数内で表示できる最大数(表示確認用) */
	UPROPERTY(VisibleAnywhere)
	int32 MaxCounterValue = 0;
};
