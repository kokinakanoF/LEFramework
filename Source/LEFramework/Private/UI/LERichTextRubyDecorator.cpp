//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================

//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LERichTextRubyDecorator.h"
#include "Fonts/FontMeasure.h"
#include "Components/RichTextBlock.h"
#include "Widgets/Layout/SScaleBox.h"


struct FRichRubyInlineData
{
public:
	FText MainText;
	FText RubyText;

	float RubyFontSize;
	float RubyHeightSpace;

	FTextBlockStyle MainTextStyle;
	FTextBlockStyle RubyTextStyle;
};




class SRichInlineRuby : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRichInlineRuby)
		{
		}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, const FRichRubyInlineData& InData)
	{
		const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
		float MainTextHeight = (float)FontMeasure->GetMaxCharacterHeight(InData.MainTextStyle.Font, 1.0f);
		float RubyTextHeight = (float)FontMeasure->GetMaxCharacterHeight(InData.RubyTextStyle.Font, 1.0f);


		ChildSlot
		[
			SNew(STextBlock)
			.Text(InData.MainText)
			.TextStyle(&InData.MainTextStyle)
		];

#if 0
		ChildSlot
		[
			SNew(SBox)
			.HeightOverride(MainTextHeight + InData.RubyHeightSpace)
			[
				SNew(SOverlay)

				// ルビのテキスト
				+ SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Top)
				[
					SNew(STextBlock)
					.Text(InData.RubyText)
					.TextStyle(&InData.RubyTextStyle)
				]

				// 本文のテキスト
				+ SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Bottom)
				[
					SNew(STextBlock)
					.Text(InData.MainText)
					.TextStyle(&InData.MainTextStyle)
				]
			]
		];
#endif
	}
};




class FRichTextRubyDecorator : public FRichTextDecorator
{
public:
	FRichTextRubyDecorator(URichTextBlock* InOwner, ULERichTextRubyDecorator* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		FString name = RunParseResult.Name;
		// 対応しているタグがある
		if (RunParseResult.MetaData.Contains(TEXT("ruby")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("ruby")];
			const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

			// ルビの設定がある
			return !FText::FromString(TagId).IsEmptyOrWhitespace();
		}

		return false;
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		if (ensure(Decorator))
		{
			FRichRubyInlineData InlineData;
			InlineData.MainText			= RunInfo.Content;
			InlineData.RubyFontSize		= Decorator->GetRubyFontSize();
			InlineData.RubyHeightSpace	= Decorator->GetRubyHeightSpace();

			// ルビのテキストを取得
			if (const FString* textString = RunInfo.MetaData.Find(TEXT("text")))
			{
				InlineData.RubyText = FText::FromString(*textString);
			}

			
			// テーブルからテキストスタイルを取得
			TOptional<FTextBlockStyle> tableTextStyle;
			if (UDataTable* styleSetTable = Owner->GetTextStyleSet())
			{
				FString ContextString;
				FRichTextStyleRow* row = styleSetTable->FindRow<FRichTextStyleRow>(*RunInfo.Name, ContextString, false);
				if (row)
				{
					tableTextStyle = row->TextStyle;
				}
			}

			// 本文のテキストスタイル取得
			InlineData.MainTextStyle = tableTextStyle.IsSet() ? tableTextStyle.GetValue() : TextStyle;

	
#if 0
			FString styleStr = RunInfo.Name;
			if (styleStr.Contains(TEXT(".Ruby")))
			{
				if (UDataTable* styleSetTable = Owner->GetTextStyleSet())
				{
					// スタイル名取得
					int32 startIndex = styleStr.Find(TEXT(".Ruby"));
					FString style = styleStr.Left(startIndex);

					// テーブルからスタイルを取得
					FString ContextString;
					FRichTextStyleRow* row = styleSetTable->FindRow<FRichTextStyleRow>(*style, ContextString, false);
					if (row)
					{
						InlineData.MainTextStyle = row->TextStyle;
					}
				}
			}
			else
			{
				InlineData.MainTextStyle = TextStyle;
			}
#endif

			// ルビの設定を上書き
			if (const FString* styleString = RunInfo.MetaData.Find(TEXT("style")))
			{
				if (FLERirchTextRubyOverrideRow* row = Decorator->FindRow(**styleString, false))
				{
					InlineData.RubyFontSize		= row->FontSize;
					InlineData.RubyHeightSpace	= row->HeightSpace;
					InlineData.RubyTextStyle	= row->TextStyle;
				}
			}
			else
			{
				InlineData.RubyTextStyle = InlineData.MainTextStyle;
			}


			InlineData.RubyTextStyle.SetFontSize(InlineData.RubyFontSize);
			return SNew(SRichInlineRuby, InlineData);
		}

		return TSharedPtr<SWidget>();
	}



private:
	ULERichTextRubyDecorator* Decorator;
};





ULERichTextRubyDecorator::ULERichTextRubyDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}


FLERirchTextRubyOverrideRow* ULERichTextRubyDecorator::FindRow(FName TagOrId, bool bWarnIfMissing)
{
	if (RubySet)
	{
		FString ContextString;
		return RubySet->FindRow<FLERirchTextRubyOverrideRow>(TagOrId, ContextString, bWarnIfMissing);
	}

	return nullptr;
}


TSharedPtr<ITextDecorator> ULERichTextRubyDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichTextRubyDecorator(InOwner, this));
}