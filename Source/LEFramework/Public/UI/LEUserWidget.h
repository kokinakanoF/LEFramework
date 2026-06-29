//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LEUserWidget.generated.h"



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLEUserWidgetEvent);



//===========================================================================
//　クラス
//　@brief UI基本クラス
//===========================================================================
UCLASS(Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULEUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** アクティブ状態にする */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void ActivateWidget();

	/** 非アクティブ状態にする */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void DeactivateWidget();

	/** @return アクティブ状態か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsActivated() const { return bIsActive; }

	/** アクティブ状態の可視性を設定 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void SetActivatedVisibility(const ESlateVisibility InVisibility) { ActivatedVisibility = InVisibility; }

	/** 非アクティブ状態の可視性を設定 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void SetDeactivatedVisibility(const ESlateVisibility InVisibility) { DeactivatedVisibility = InVisibility; }


	/** ポーズ設定を行う */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWidgetPaused(const bool bPaused);

	/** @return ポーズ中か？ */
	UFUNCTION(BlueprintCallable)
	bool IsWidgetPaused() const { return bIsPaused; }



protected:
	/** アクティブ状態になった */
	UFUNCTION(BlueprintImplementableEvent, Category = "LE Framework|UI")
	void BP_OnActivated();
	virtual void NativeOnActivated();

	/** 非アクティブ状態になった */
	UFUNCTION(BlueprintImplementableEvent, Category = "LE Framework|UI")
	void BP_OnDeactivated();
	virtual void NativeOnDeactivated();

	/**
	 * いづれかのキーダウン入力を受け取った
	 * @param InGeometry	- 自身のジオメトリ
	 * @param InKey			- 入力キー
	 * @return イベント処理結果
	 */
	UFUNCTION(BlueprintImplementableEvent)
	FEventReply OnAnyButtonDown(const FGeometry& InGeometry, const FKey& InKey);
	virtual FReply NativeOnAnyButtonDown(const FGeometry& InGeometry, const FKey& InKey);
	
	/** 
	 * いづれかのキーアップ入力を受け取った 
	 * @param InGeometry	- 自身のジオメトリ
	 * @param InKey			- 入力キー
	 * @return イベント処理結果
	 */
	UFUNCTION(BlueprintImplementableEvent)
	FEventReply OnAnyButtonUp(const FGeometry& InGeometry, const FKey& InKey);
	virtual FReply NativeOnAnyButtonUp(const FGeometry& InGeometry, const FKey& InKey);


	/**
	 * ポーズによって停止される更新処理
	 * @param MyGeometry	- 自身のジオメトリ
	 * @param InDeltaTime	- 単位時間
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void PausedTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativePausedTick(const FGeometry& MyGeometry, float InDeltaTime);


protected:
	/** アニメーションのポーズ */
	void SetPauseAnimations(const bool bPause);


protected:
	//~Begin UUserWidget interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	//~End UUserWidget interface



public:
	/** アクティブ化時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEUserWidget|Event")
	FOnLEUserWidgetEvent OnActivated;

	/** 非アクティブ化時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEUserWidget|Event")
	FOnLEUserWidgetEvent OnDeactivated;


protected:
	/** Construct時に自動でアクティブ化するか？ */
	UPROPERTY(EditAnywhere, Category = "LEUserWidget|Behavior")
	bool bAutoActivate = false;

	/** ポーズによって停止するか？ */
	UPROPERTY(EditAnywhere, Category = "LEUserWidget|Behavior")
	bool bEnablePause = false;


	/** アクティブ時に可視性を設定するか？ */
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle = "ActivatedVisibility"), Category = "LEUserWidget|Behavior")
	bool bSetVisibilityOnActivated = false;

	/** アクティブ状態の可視性 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bSetVisibilityOnActivated"), Category = "LEUserWidget|Behavior")
	ESlateVisibility ActivatedVisibility = ESlateVisibility::SelfHitTestInvisible;

	/** 非アクティブ時に可視性を設定するか？ */
	UPROPERTY(EditAnywhere, meta = (InlineEditConditionToggle = "DeactivatedVisibility"), Category = "LEUserWidget|Behavior")
	bool bSetVisibilityOnDeactivated = false;

	/** 非アクティブ状態の可視性 */
	UPROPERTY(EditAnywhere, meta = (EditCondition = "bSetVisibilityOnDeactivated"), Category = "LEUserWidget|Behavior")
	ESlateVisibility DeactivatedVisibility = ESlateVisibility::Collapsed;


private:
	/** アクティブ状態 */
	bool bIsActive = false;

	/** ポーズ状態 */
	bool bIsPaused = false;
};
