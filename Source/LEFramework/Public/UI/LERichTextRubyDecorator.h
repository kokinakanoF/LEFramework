//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "Engine/DataTable.h"
#include "LERichTextRubyDecorator.generated.h"



//===========================================================================
//　構造体
//===========================================================================
/** 
 * ルビ表示の上書きデータ
 * @note フォントサイズやテキストスタイルをルビ用に上書きできます
 */
USTRUCT(BlueprintType)
struct FLERirchTextRubyOverrideRow
{
	GENERATED_BODY()

public:
	/** 本文のテキストサイズに対するスケール値 */
	UPROPERTY(EditAnywhere, Category = Appearance)
	float FontSize = 12.0f;

	/** 本文に対する高さのオフセット */
	UPROPERTY(EditAnywhere, Category = Appearance)
	float HeightSpace = 5.0f;

	/** 上書きするスタイル */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FTextBlockStyle TextStyle;
};



//===========================================================================
//　クラス
//　@brief ルビ表示用デコレータ
//===========================================================================
UCLASS()
class ULERichTextRubyDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	ULERichTextRubyDecorator(const FObjectInitializer& ObjectInitializer);

	/** @return テーブルから取得した行 */
	FLERirchTextRubyOverrideRow* FindRow(FName TagOrId, bool bWarnIfMissing);

	float GetRubyFontSize() const { return RubyFontSize; }
	float GetRubyHeightSpace() const { return RubyHeightSpace; }

public:
	//~Begin URichTextBlockDecorator interface
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	//~End URichTextBlockDecorator interface



protected:
	/** ルビテーブル */
	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=/Script/LEFramework.LERirchTextRubyOverrideRow"))
	TObjectPtr<class UDataTable> RubySet;

	/** ルビのフォントサイズ */
	UPROPERTY(EditAnywhere, Category = Appearance)
	float RubyFontSize = 12.0f;

	/** 本文に対するルビのオフセット値 */
	UPROPERTY(EditAnywhere, Category = Appearance)
	float RubyHeightSpace = 5.0f;
};
