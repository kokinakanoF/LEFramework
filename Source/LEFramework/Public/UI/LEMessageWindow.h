//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UI/LEInOutWidget.h"
#include "LEMessageWindow.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class UTextLayoutWidget;


//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLEMessageWindowEvent);



//===========================================================================
//　構造体
//===========================================================================
#if 0
USTRUCT(BlueprintType)
struct FLEMessageData
{
	GENERATED_BODY()

	/**
	 * テキストID
	 * @note 話者名やボイスと紐づけたい場合は
	 *		 TextIDをキーとしたシステムを構築してください
	 */
	FString TextID = "";
};
#endif



//===========================================================================
//　クラス
//　@brief 会話ウィンドウ
//===========================================================================
UCLASS()
class LEFRAMEWORK_API ULEMessageWindow : public ULEInOutWidget
{
	GENERATED_BODY()
	
protected:
	enum class EMessageState
	{
		Going,
		After,
		Finished
	};

public:
#if 0
	/** メッセージデータを設定する */
	UFUNCTION(BlueprintNativeEvent)
	void SetMessageData(const FLEMessageData& InData);
#endif

	/** メッセージが終了したか？ */
	UFUNCTION(BlueprintCallable)
	bool IsMessageFinished() const { return MessageState == EMessageState::Finished; }

	/** オート再生の通知を受け取った */
	UFUNCTION(BlueprintNativeEvent)
	void ReceiveAutoPlay(const bool bAutoPlay);
	virtual void ReceiveAutoPlay_Implementation(const bool bAutoPlay);

protected:
	/**
	 * ウィジェットにテキストを設定する
	 * 
	 * @param InWidget	テキスト設定対象のウィジェット
	 * @param InText	設定するテキスト
	 */
	void SetTextToWidget(TObjectPtr<UTextLayoutWidget> InWidget, const FText& InText);


	virtual void TickMessageGoing(float InDeltaTime);
	virtual void TickMessageAfter(float InDeltaTime);

protected:
	//~Begin UUserWidget interface
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget interface

	//~Begin UAOInOutWidget interface
	virtual void NativeFinishClose() override;
	//~End UAOInOutWidget interface


public:
	/** メッセージ開始時イベント */
	UPROPERTY(BlueprintAssignable)
	FOnLEMessageWindowEvent OnMessageStarted;

	/** メッセージ終了時イベント */
	UPROPERTY(BlueprintAssignable)
	FOnLEMessageWindowEvent OnMessageFinished; 


protected:
	/** 話者名テキスト */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget), Category = "LE Framework|UI")
	TObjectPtr<UTextLayoutWidget> Txt_SpeakerName;

	/** 本文 */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidget), Category = "LE Framework|UI")
	TObjectPtr<UTextLayoutWidget> Txt_Message;


protected:
	EMessageState MessageState = EMessageState::Finished;

	float MessageWaitTime;
	float MessageAfterWaitTime;
};
