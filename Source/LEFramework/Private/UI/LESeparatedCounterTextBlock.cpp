//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "UI/LESeparatedCounterTextBlock.h"



ULESeparatedCounterTextBlock::ULESeparatedCounterTextBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TextFormat = FText::FromString("<Dark>{0}</><Bright>{1}</>");
}


void ULESeparatedCounterTextBlock::SetCounterValue(int32 InValue)
{
	const int32 value = FMath::Min(InValue, MaxCounterValue);
	const FString valueStr = FString::FromInt(InValue);

	// ゼロ埋めテキストの作成
	FString zeroStr = FString("").LeftPad(MaxDigitNum - valueStr.Len());
	zeroStr = zeroStr.Replace(TEXT(" "), TEXT("0"));

	// テキストの設定
	FText newText = TextFormat;
	newText = FText::Format(newText, FText::FromString(zeroStr), FText::FromString(valueStr));
	SetText(newText);
}



void ULESeparatedCounterTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	// 最大数値の計算
	if (MaxDigitNum >= 1)
	{
		MaxCounterValue = 1;
		for (int32 i = 0; i < MaxDigitNum; i++)
		{
			MaxCounterValue *= 10;
		}
		MaxCounterValue--;
	}
	else
	{
		MaxCounterValue = 1;
	}
}