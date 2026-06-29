//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "UI/Interaction/LESelectableWidget.h"
#include "Types/SlateEnums.h"
#include "LEButton.generated.h"



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLEButtonEvent, ULEButton*, Button);



//===========================================================================
//　列挙体
//===========================================================================
 /** ボタン挙動のタイプ */
UENUM(BlueprintType)
enum class ELEButtonPressAnimPlayTimming : uint8
{
	Clicked,
	PressAndRelease
};




//===========================================================================
//　クラス
//　@brief ボタンのベースクラス
//===========================================================================
UCLASS(Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULEButton : public ULESelectableWidget
{
	GENERATED_BODY()

public:
	/** @return 押されている状態か？ */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsPressed() const { return bIsPressed; }

	/** プレス可能設定 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	bool SetIsPressable(bool bPressable) { return bIsPressable = bPressable; }

	/** @return プレス可能設定 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "LE Framework|UI")
	bool IsPressable() const { return bIsPressable; }


protected:
	/** プレス処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Pressed"), Category = "LE Framework|UI")
	void BP_OnPressed();

	/** リリース処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Releaseed"), Category = "LE Framework|UI")
	void BP_OnReleased();
	
	/** クリック処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Clicked"), Category = "LE Framework|UI")
	void BP_OnClicked();
	

protected:
	void SetIsPressed(bool bPressed);

	virtual void NativeOnPressed();
	virtual void NativeOnReleased();
	virtual void NativeOnClicked();

	virtual void PlayPressedAnim();
	virtual void PlayReleasedAnim();
	virtual void PlayClickedAnim();

	/**
	 * 入力方法からクリック方法を取得します
	 * 
	 * @param InMouseEvent - マウスイベント
	 * @return クリック方法
	 */
	EButtonClickMethod::Type GetClickMethodFromInputType(const FPointerEvent& InMouseEvent) const;

	/**
	 * 正確なクリックやタップが必要か取得します
	 * 
	 * @param InMouseEvent - マウスイベント
	 * @return 正確なクリックやタップが必要
	 */
	bool IsPreciseTapOrClick(const FPointerEvent& InMouseEvent) const;

protected:
	//~Begin UUserWidget interface
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	//~End UUserWidget interaface


public:
	/** プレス時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEButtonBase|Event")
	FOnLEButtonEvent OnPressed;

	/** リリース時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LEButtonBase|Event")
	FOnLEButtonEvent OnReleased;

	/** クリック時時イベント */ 
	UPROPERTY(BlueprintAssignable, Category = "LEButtonBase|Event")
	FOnLEButtonEvent OnClicked;

protected:
	/** AN_Pressアニメーションの作成タイプ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase")
	ELEButtonPressAnimPlayTimming PressAnimPlayTimming;

	/** マウスクリック時のイベント応答方法 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase|Interaction")
	TEnumAsByte<EButtonClickMethod::Type> MouseClickMethod;

	/** タッチ時のイベント応答方法 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase|Interaction")
	TEnumAsByte<EButtonTouchMethod::Type> TouchMethod;

	/** キーボードやパッドのキー入力時のイベント応答方法 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase|Interaction")
	TEnumAsByte<EButtonPressMethod::Type> KeyPressMethod;

	/** プレス時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase|Sound", meta = (EditCondition = "ButtonBehavior != ELEButtonBehavior::OneShotRelease"))
	FSlateSound PressedSound;

	/** クリック時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LEButtonBase|Sound")
	FSlateSound ClickedSound;

protected:
	/**
	 * プレスアニメーション
	 * PressAnimTypeがPingPongの場合は、逆再生を想定した一方通行のアニメーションである必要があります
	 */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "LEButton")
	TObjectPtr<UWidgetAnimation> AN_Pressed;


private:
	bool bIsPressable = true;
	bool bIsPressed = false;
};


