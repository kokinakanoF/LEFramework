//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UI/LEUserWidget.h"
#include "LEInOutWidget.generated.h"



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLEInOutWidgetEvent);



//===========================================================================
//　列挙体
//===========================================================================
/** 入退場状態 */
UENUM(BlueprintType, Category = "LEInOutWidget")
enum class ELEInOutState : uint8
{
	CloseFinished	UMETA(DisplayName = "退場完了"),
	Opening			UMETA(DisplayName = "入場中"),
	OpenFinished	UMETA(DisplayName = "入場完了"),
	Closing			UMETA(DisplayName = "退場中")
};

/** アニメーションを上書き再生する際の設定 */
UENUM(BlueprintType, Category = "LEInOutWidget")
enum class ELEInOutOverrideSetting : uint8
{
	Always			UMETA(DisplayName = "いつでも"),
	ToggleOnly		UMETA(DisplayName = "トグルする場合のみ"),
	AnimCompleted	UMETA(DisplayName = "アニメ完了時")
};



//===========================================================================
//　クラス
//　@brief 入退場可能なウィジェット
//===========================================================================
UCLASS(Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULEInOutWidget : public ULEUserWidget
{
	GENERATED_BODY()
	
public:
	/** 入場処理 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void Open();

	/** 退場処理 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void Close();

	/** 入退場をトグルする */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void ToggleInOut();


	/** @return 入場中か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsOpening() const { return (IOState == ELEInOutState::Opening); }

	/** @return 入場が完了しているか？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsOpenFinished() const { return (IOState == ELEInOutState::OpenFinished); }

	/** @return 入場状態か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsOpened() const { return IsOpening() || IsOpenFinished(); }

	/** @return 退場中か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsClosing() const { return (IOState == ELEInOutState::Closing); }

	/** @return 退場が完了しているか？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsCloseFinished() const { return (IOState == ELEInOutState::CloseFinished); }

	/** @return 退場しているか？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsClosed() const { return IsClosing() || IsCloseFinished(); }

	/** @return 入場可能か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsEnableOpen();

	/** @return 退場可能か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsEnableClose();

	/** @return 入退場状態 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	ELEInOutState GetIOState() const { return IOState; }


protected:
	/** 入場開始時処理 */
	UFUNCTION(BlueprintImplementableEvent, Category = "LEInOutWidget|Event")
	void StartOpen();
	virtual void NativeStartOpen();

	/** 入場終了時処理 */
	UFUNCTION(BlueprintImplementableEvent, Category = "LEInOutWidget|Event")
	void FinishOpen();
	virtual void NativeFinishOpen();

	/** 退場開始時処理 */
	UFUNCTION(BlueprintImplementableEvent, Category = "LEInOutWidget|Event")
	void StartClose();
	virtual void NativeStartClose();

	/** 退場終了時処理 */
	UFUNCTION(BlueprintImplementableEvent, Category = "LEInOutWidget|Event")
	void FinishClose();
	virtual void NativeFinishClose();


protected:
	//~Begin UUserWidget interface
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	//~End UUserWidget interface


private:
	void PlayOpenAnim();
	void PlayCloseAnim();




public:
	/** 入場開始時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEInOutWidget|Event")
	FOnLEInOutWidgetEvent OnStartOpen;

	/** 入場終了時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEInOutWidget|Event")
	FOnLEInOutWidgetEvent OnFinishOpen;

	/** 退場開始時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEInOutWidget|Event")
	FOnLEInOutWidgetEvent OnStartClose;

	/** 退場終了時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEInOutWidget|Event")
	FOnLEInOutWidgetEvent OnFinishClose;




protected:
	/** 入場アニメーション */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "LEInOutWidget|Animation")
	TObjectPtr<UWidgetAnimation> AN_Open;

	/** 退場アニメーション */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional), Category = "LEInOutWidget|Animation")
	TObjectPtr<UWidgetAnimation> AN_Close;


protected:
	/** デフォルトが入場状態である */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LEInOutWidget|Appearance")
	bool bDefaultOpen = false;

	/** 入退場の上書き設定 */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LEInOutWidget|Appearance")
	ELEInOutOverrideSetting OverrideSetting = ELEInOutOverrideSetting::Always;

	/** Construct時に自動でOpenするか？ */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LEInOutWidget|Appearance")
	bool bAutoOpen = false;

	/** FinishClose時に自動でRemoveFromParentするか？ */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "LEInOutWidget|Appearance")
	bool bAutoDestruct = false;

	/** オープン時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEInOutWidget|Sound")
	FSlateSound OpenSound;

	/** クローズ時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEInOutWidget|Sound")
	FSlateSound CloseSound;

private:
	/** 入退場状態 */
	ELEInOutState IOState = ELEInOutState::CloseFinished;
};
