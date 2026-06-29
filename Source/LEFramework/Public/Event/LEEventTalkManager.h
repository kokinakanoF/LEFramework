//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/LEMessageWindow.h"
#include "LEEventTalkManager.generated.h"



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLEEventTalkEvent);



//===========================================================================
//　デリゲート
//===========================================================================
/** 会話の１メッセージデータ */
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




//===========================================================================
//　クラス
//　@brief [イベント]会話マネージャ
//===========================================================================
UCLASS(BlueprintType)
class LEFRAMEWORK_API ULEEventTalkManager : public UObject
{
	GENERATED_BODY()
	
public:
	/** 会話データの単体追加 */
	UFUNCTION(BlueprintCallable)
	void AddTalkData(const FLEMessageData& InData);

	/** 会話データの複数追加 */
	UFUNCTION(BlueprintCallable)
	void AddTalkDataArray(const TArray<FLEMessageData>& InData);

	/** 会話データのクリア */
	UFUNCTION(BlueprintCallable)
	void ClearTalkData();

	/** 会話のキャンセル */
	UFUNCTION(BlueprintCallable)
	void CancelTalk();

	/** 次のメッセージへ進む */
	UFUNCTION(BlueprintCallable)
	void NextMessage();

	/** 会話中か？ */
	UFUNCTION(BlueprintCallable)
	bool IsTalking() const;


	/** @return 会話データがあるか？ */
	UFUNCTION(BlueprintCallable)
	bool HasTalkData() const { return !TalkData.IsEmpty(); }

	/** @return 会話データ */
	UFUNCTION(BlueprintCallable)
	TArray<FLEMessageData> GetAllTalkData() const { return TalkData; }

	/** @return 先頭の会話データ(または表示中の会話データ) */
	UFUNCTION(BlueprintCallable)
	FLEMessageData GetTopTalkData() const { return TalkData.IsEmpty() ? FLEMessageData() : TalkData[0]; }

	/** 
	 * 会話ウィンドウの登録
	 * @note 現在登録されているウィンドウは登録解除されます
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterMessageWindow(ULEMessageWindow* InWindow);

	/** 
	 * 会話ウィンドウの登録解除
	 * @note 現在行われている会話は停止されます
	 */
	UFUNCTION(BlueprintCallable)
	void UnregisterMessageWindow();

	/** オート再生の設定 */
	UFUNCTION(BlueprintCallable)
	void SetAutoPlay(const bool bEnable);


protected:
	/** 会話開始 */
	virtual void StartTalk();

	/** 会話終了 */
	virtual void EndTalk();

	/** メッセージの開始 */
	virtual void StartMessage(const FLEMessageData& InData);



public:
	/** 会話開始時イベント */
	UPROPERTY(BlueprintAssignable)
	FOnLEEventTalkEvent OnTalkStart;

	/** 会話終了時イベント */
	UPROPERTY(BlueprintAssignable)
	FOnLEEventTalkEvent OnTalkEnd;

protected:
	/** データ設定先のメッセージウィンドウ */
	UPROPERTY()
	TObjectPtr<ULEMessageWindow> MessageWindow;


	TArray<FLEMessageData> TalkData;
	bool bAutoPlay = false;
};
