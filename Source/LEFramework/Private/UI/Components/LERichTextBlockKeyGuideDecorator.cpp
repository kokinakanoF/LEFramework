//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/Components/LERichTextBlockKeyGuideDecorator.h"
#include "UObject/SoftObjectPtr.h"
#include "Rendering/DrawElements.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DefaultValueHelper.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/Package.h"
// Slate
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Framework/Text/SlateTextRun.h"
#include "Framework/Text/SlateTextLayout.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SBox.h"
// Widget
#include "Components/Widget.h"
// Font
#include "Engine/Font.h"
#include "Fonts/FontMeasure.h"



struct FRichInlineData
{
	FRichInlineData(const FText InText, const FTextBlockStyle& InTextStyle, TOptional<int32> InWidth, TOptional<int32> InHeight, EStretch::Type InStretch)
		: Text(InText)
		, TextStyle(InTextStyle)
		, Width(InWidth)
		, Height(InHeight)
		, Stretch(InStretch)
	{

	}

	FText Text;
	FTextBlockStyle TextStyle;
	TOptional<int32> Width;
	TOptional<int32> Height;
	EStretch::Type Stretch;
};




class SRichInlineImageAndText : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineImageAndText)
		{
		}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const FLERichKeyGuideRow* Row, const FRichInlineData& Data, float IconScale, const FLERichImageTextInfo& TextSettings)
	{
		check(Row);

		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float IconHeight = FMath::Min((float)FontMeasure->GetMaxCharacterHeight(Data.TextStyle.Font, 1.0f), Row->Brush.ImageSize.Y);

		// 高さの設定確認
		if (Data.Height.IsSet())
		{
			IconHeight = Data.Height.GetValue();
		}

		// 幅の設定確認
		float IconWidth = IconHeight;
		if (Data.Width.IsSet())
		{
			IconWidth = Data.Width.GetValue();
		}

		// アイコンスケール適用
		IconWidth *= IconScale;
		IconHeight *= IconScale;


		// スレート作成
		ChildSlot
		[
			SNew(SBox)
			.HeightOverride(IconHeight)
			.WidthOverride(IconWidth)
			[
				SNew(SOverlay)

				// アイコン画像
				+ SOverlay::Slot()
				[
					SNew(SImage)
					.Image(&Row->Brush)
				]

				// アイコンの上に乗せるテキスト
				// ※主にキーボード用
				+ SOverlay::Slot()
				.HAlign(TextSettings.HorizontalAlignment)
				.VAlign(TextSettings.VerticalAlignment)
				[
					SNew(SScaleBox)
					.VAlign(EVerticalAlignment::VAlign_Center)
					.Stretch(EStretch::Type::ScaleToFit)
					[
						SNew(STextBlock)
						.Text(Data.Text)
						.Font(TextSettings.Font)
						.StrikeBrush(&TextSettings.StrikeBrush)
						.ColorAndOpacity(TextSettings.ColorAndOpacity)
						.ShadowOffset(TextSettings.ShadowOffset)
						.ShadowColorAndOpacity(TextSettings.ShadowColorAndOpacity)
						.TransformPolicy(TextSettings.TransformPolicy)
						.Margin(TextSettings.Margin)
						.Justification(ETextJustify::Center)
					]
				]
			]
		];
	}
};



class FRichInlineImage : public FRichTextDecorator
{
public:
	FRichInlineImage(URichTextBlock* InOwner, ULERichTextBlockKeyGuideDecorator* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("img") && RunParseResult.MetaData.Contains(TEXT("id")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
			const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

			// 指定した行がテーブルにあればOK
			const bool bWarnIfMissing = false;
			return Decorator->FindLETableRow(*TagId, bWarnIfMissing) != nullptr;
		}

		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		const bool bWarnIfMissing = true;
		const FLERichKeyGuideRow* row = Decorator->FindLETableRow(*RunInfo.MetaData[TEXT("id")], bWarnIfMissing);
		
		if (ensure(row))
		{
			TOptional<int32> Width;
			if (const FString* WidthString = RunInfo.MetaData.Find(TEXT("width")))
			{
				int32 WidthTemp;
				if (FDefaultValueHelper::ParseInt(*WidthString, WidthTemp))
				{
					Width = WidthTemp;
				}
				else
				{
					if (FCString::Stricmp(GetData(*WidthString), TEXT("desired")) == 0)
					{
						Width = row->Brush.ImageSize.X;
					}
				}
			}

			TOptional<int32> Height;
			if (const FString* HeightString = RunInfo.MetaData.Find(TEXT("height")))
			{
				int32 HeightTemp;
				if (FDefaultValueHelper::ParseInt(*HeightString, HeightTemp))
				{
					Height = HeightTemp;
				}
				else
				{
					if (FCString::Stricmp(GetData(*HeightString), TEXT("desired")) == 0)
					{
						Height = row->Brush.ImageSize.Y;
					}
				}
			}

			EStretch::Type Stretch = EStretch::ScaleToFit;
			if (const FString* SstretchString = RunInfo.MetaData.Find(TEXT("stretch")))
			{
				const UEnum* StretchEnum = StaticEnum<EStretch::Type>();
				int64 StretchValue = StretchEnum->GetValueByNameString(*SstretchString);
				if (StretchValue != INDEX_NONE)
				{
					Stretch = static_cast<EStretch::Type>(StretchValue);
				}
			}

			FText Text = RunInfo.Content;
			if (!row->TextOverride.IsEmpty())
			{
				Text = row->TextOverride;
			}

			const FLERichImagePlatformInfo& platformInfo = Decorator->GetPlatformInfo();

			FRichInlineData InlineData(Text, TextStyle, Width, Height, Stretch);
			return SNew(SRichInlineImageAndText, row, InlineData, platformInfo.IconScaleByTextSize, platformInfo.TextSettings);
		}
		return TSharedPtr<SWidget>();
	}

private:
	ULERichTextBlockKeyGuideDecorator* Decorator;
};





FLERichImageTextInfo::FLERichImageTextInfo()
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	HorizontalAlignment		= HAlign_Center;
	VerticalAlignment		= VAlign_Center;
	ColorAndOpacity			= FLinearColor::White;
	ShadowOffset			= FVector2D(1.0f, 1.0f);
	ShadowColorAndOpacity	= FLinearColor::Transparent;
	TransformPolicy			= ETextTransformPolicy::None;
	Margin					= FMargin(0.0f);

#if 0
	// デフォルトのフォントを設定しておく
	if (!IsRunningDedicatedServer())
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		Font = FSlateFontInfo(RobotoFontObj.Object, 24, FName("Bold"));
	}
#endif

PRAGMA_ENABLE_DEPRECATION_WARNINGS
}



ULERichTextBlockKeyGuideDecorator::ULERichTextBlockKeyGuideDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FLERichImagePlatformInfo ULERichTextBlockKeyGuideDecorator::GetPlatformInfo() const
{
#if WITH_EDITOR
	// プレビュー用テーブルを使用
	return PreviewInfo;
#else
	// プラットフォームに応じたテーブルを使用
	const FString platformName = FPlatformProperties::PlatformName();
	return  PlatformInfo.FindRef(platformName);
#endif
}

FLERichKeyGuideRow* ULERichTextBlockKeyGuideDecorator::FindLETableRow(FName TagOrId, bool bWarnIfMissing)
{
	// テーブルからデータを取り出す
	const FLERichImagePlatformInfo platformInfo = GetPlatformInfo();
	if (platformInfo.DataTable)
	{
		FString ContextString;
		return platformInfo.DataTable->FindRow<FLERichKeyGuideRow>(TagOrId, ContextString, bWarnIfMissing);
	}

	return nullptr;
}

FLERichImageTextInfo ULERichTextBlockKeyGuideDecorator::GetTextSettings() const
{
	return GetPlatformInfo().TextSettings;
}


TSharedPtr<ITextDecorator> ULERichTextBlockKeyGuideDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichInlineImage(InOwner, this));
}