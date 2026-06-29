//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LEEventObject.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class ULevelSequence;
class UFileMediaSource;



//===========================================================================
//　列挙体
//===========================================================================
/** イベントの種別 */
UENUM(BlueprintType)
enum class ELEEventType : uint8
{
	None				UMETA(DisplayName = "なし"),
	Message				UMETA(DisplayName = "メッセージ"),
	Choice				UMETA(DisplayName = "選択肢"),
	Sequencer			UMETA(DisplayName = "シーケンサー"),
	Movie				UMETA(DisplayName = "ムービー"),
	Custom				UMETA(DisplayName = "カスタム"),
};



//===========================================================================
//　構造体
//===========================================================================
/** メッセージイベント用データ */
USTRUCT(BlueprintType)
struct FLEEventMessageData
{
	GENERATED_BODY()

public:
	/** プレイアブルか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsPlayable = true;
};

/** 選択肢イベント用データ */
USTRUCT(BlueprintType)
struct FLEEventChoiceData
{
	GENERATED_BODY()
};

/** シーケンサーイベント用データ */
USTRUCT(BlueprintType)
struct FLEEventSequencerData
{
	GENERATED_BODY()

public:
	/** シーケンサーアセット */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<ULevelSequence> Asset;
};

/** ムービーイベント用データ */
USTRUCT(BlueprintType)
struct FLEEventMovieData
{
	GENERATED_BODY()

public:
	/** 動画アセット */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UFileMediaSource> Asset;
};



//===========================================================================
//　クラス
//　@brief ボタンのベースクラス
//===========================================================================
UCLASS()
class LEFRAMEWORK_API ALEEventObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ALEEventObject();

protected:
	/** イベントタイプ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELEEventType EventType = ELEEventType::None;

	/** メッセージイベント用データ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EventType == ELEEventType::Message"))
	FLEEventMessageData MessageData;

	/** 選択肢イベント用データ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EventType == ELEEventType::Choice"))
	FLEEventChoiceData ChoiceData;

	/** シーケンサーイベント用データ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EventType == ELEEventType::Sequencer"))
	FLEEventSequencerData SequencerData;

	/** 動画イベント用データ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "EventType == ELEEventType::Movie"))
	FLEEventMovieData MovieData;

	/**
	 * カスタムイベント用データ 
	 * @note 継承したクラスで独自に作成してください
	 */
};
