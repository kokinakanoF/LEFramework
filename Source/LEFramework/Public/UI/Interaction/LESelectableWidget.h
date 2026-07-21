//===========================================================================
// Copyright Koki Nakano. All Rights Reserved.
//===========================================================================
#pragma once
//===========================================================================
//　インクルード
//===========================================================================
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LESelectableWidget.generated.h"



//===========================================================================
//　前方宣言
//===========================================================================
class UTextLayoutWidget;



//===========================================================================
//　列挙体
//===========================================================================
/** 選択方法 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ELESelectMethod : uint8
{
	None = 0					UMETA(Hidden),
	Focus = 1 << 0				UMETA(DisplayName = "フォーカス"),
	Mouse = 1 << 1				UMETA(DisplayName = "マウスカーソル"),

	All = (Focus | Mouse)		UMETA(Hidden)
};
ENUM_CLASS_FLAGS(ELESelectMethod)



//===========================================================================
//　デリゲート
//===========================================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLESelectableWidgetEvent, ULESelectableWidget*, Widget);



//===========================================================================
//　クラス
//　@brief マウスカーソルやナビゲーションでフォーカス可能なウィジェット
//===========================================================================
UCLASS(Abstract, ClassGroup = UI, meta = (Category = "LE Framework"))
class LEFRAMEWORK_API ULESelectableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	ULESelectableWidget(const FObjectInitializer& ObjectInitializer);

public:
	/** 選択可能設定 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void SetIsSelectable(bool bEnable);

	/** @return 選択可能か？ */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	bool IsSelectable() const { return bIsSelectable; }


	/** @return 選択状態か？ */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void IsSelected() { bIsSelected; }


	/**
	 * ラベルテキストの設定
	 *
	 * @param InText 表示するテキスト
	 */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	void SetLabelText(const FText& InText);

	/** @return ラベルテキスト */
	UFUNCTION(BlueprintCallable, Category = "LE Framework|UI")
	FText GetLabelText() const { return LabelText; }


protected:
	/** 選択処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Selected"), Category = "LEFramework")
	void BP_OnSelected();

	/** 非選択処理 */
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Deselected"), Category = "LEFramework")
	void BP_OnDeselected();

protected:
	void SetIsSelected(bool bSelected);

	virtual void NativeOnSelected();
	virtual void NativeOnDeselected();

	virtual void PlaySelectedAnimation();
	virtual void PlayDeselectedAnimation();

	/** ラベルテキストの表示更新 */
	virtual void RefreshLabelTextBlock();


protected:
	//~Begin UUserWidget interface
	virtual void SynchronizeProperties() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	//~End UUserWidet interface



public:
	/** 選択時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LESelectableWidget|Event")
	FOnLESelectableWidgetEvent OnSelected;

	/** 選択解除時イベント */
	UPROPERTY(BlueprintAssignable, Category = "LESelectableWidget|Event")
	FOnLESelectableWidgetEvent OnDeselected;


protected:
	/** 選択方法 */
	UPROPERTY(EditAnywhere, meta = (Bitmask, BitmaskEnum = "/Script/LEFramework.ELESelectMethod"), Category = "LESelectableWidget")
	uint8 SelectMethod = (uint8)ELESelectMethod::All;

	/**
	 * ラベルテキスト
	 * Txt_LabelというTextBlock/RichTextBlockが存在する場合のみ使用可能です
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "Txt_Label != nullptr"), Category = "LESelectableWidget|Appearance")
	FText LabelText;

	/** 選択時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LESelectableWidget|Sound")
	FSlateSound SelectedSound;

	/** 選択解除時に再生するSE */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LESelectableWidget|Sound")
	FSlateSound DeselectedSound;


protected:
	/**
	 * ラベルテキストブロック
	 * UTextBlock/URichTextBlockのいづれでも使用可能です
	 */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetOptional), Category = "LESelectableWidget")
	TObjectPtr<UTextLayoutWidget> Txt_Label;

	/** 
	 * 選択アニメーション 
	 * AN_Deselectedがない場合は、非選択時にAN_Selectedを逆再生します
	 */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim), Category = "LESelectableWidget|Appearance")
	TObjectPtr<UWidgetAnimation> AN_Selected;

	/** 選択解除アニメーション */
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnimOptional), Category = "LESelectableWidget|Appearance")
	TObjectPtr<UWidgetAnimation> AN_Deselected;


protected:
	bool bIsSelectable = true;
	bool bIsSelected = false;
};
