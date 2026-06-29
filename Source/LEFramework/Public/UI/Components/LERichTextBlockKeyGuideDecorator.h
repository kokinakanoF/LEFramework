//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateColor.h"
#include "Types/SlateEnums.h"
#include "Engine/DataTable.h"
#include "LERichTextBlockKeyGuideDecorator.generated.h"



//===========================================================================
//　構造体
//===========================================================================
/** 画像の上に表示するテキスト情報 */
USTRUCT(Blueprintable, BlueprintType)
struct FLERichImageTextInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FLERichImageTextInfo();

public:
	/** 左右のアラインメント */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment;

	/** 上下のアラインメント */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EVerticalAlignment> VerticalAlignment;

	/** テキストのフォント */
	UPROPERTY(EditAnywhere)
	FSlateFontInfo Font;

	/** ストライクブラシ */
	UPROPERTY(EditAnywhere)
	FSlateBrush StrikeBrush;

	/** 文字色 */
	UPROPERTY(EditAnywhere)
	FSlateColor ColorAndOpacity;

	/** 影のオフセット */
	UPROPERTY(EditAnywhere)
	FVector2D ShadowOffset;

	/** 影の色 */
	UPROPERTY(EditAnywhere)
	FLinearColor ShadowColorAndOpacity;

	/** 配置ルール */
	UPROPERTY(EditAnywhere)
	ETextTransformPolicy TransformPolicy;

	/** マージン */
	UPROPERTY(EditAnywhere)
	FMargin Margin;
};


/** テキスト付きガイドを表示できるデータ */
USTRUCT(Blueprintable, BlueprintType)
struct FLERichKeyGuideRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	/** 表示する画像 */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush Brush;

	/** 固定で表示するテキスト */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FText TextOverride;
};


/** 画像の上に表示するテキスト情報 */
USTRUCT(Blueprintable, BlueprintType)
struct FLERichImagePlatformInfo
{
	GENERATED_USTRUCT_BODY()

public:
	/** データテーブル */
	UPROPERTY(EditAnywhere, meta = (RequiredAssetDataTags = "RowStructure=/Script/LEFramework.LERichKeyGuideRow"))
	TObjectPtr<class UDataTable> DataTable;

	/** テキストサイズに対するアイコンのスケール */
	UPROPERTY(EditAnywhere)
	float IconScaleByTextSize = 1.0f;

	/** テキストの共通設定 */
	UPROPERTY(EditAnywhere)
	FLERichImageTextInfo TextSettings;
};



//===========================================================================
//　クラス
//　@brief 拡張版画像デコレータ
//===========================================================================
UCLASS()
class LEFRAMEWORK_API ULERichTextBlockKeyGuideDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	ULERichTextBlockKeyGuideDecorator(const FObjectInitializer& ObjectInitializer);

	/** @return プラットフォーム用データ */
	FLERichImagePlatformInfo GetPlatformInfo() const;

	/** @return テーブルから取得したFLERichImageRow */
	FLERichKeyGuideRow* FindLETableRow(FName TagOrId, bool bWarnIfMissing);

	/** @return テキストデータ */
	FLERichImageTextInfo GetTextSettings() const;

public:
	//~Begin URichTextBlockDecorator interface
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	//~End URichTextBlockDecorator interface



protected:
#if WITH_EDITORONLY_DATA
	/** プレビュー用情報 */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FLERichImagePlatformInfo PreviewInfo;
#endif

	/** プラットフォームごとの情報 */
	UPROPERTY(EditAnywhere, Category = Appearance)
	FLERichImagePlatformInfo PlatformInfo;
};
