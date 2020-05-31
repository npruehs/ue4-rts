#pragma once

#include "DaeTestComparisonMethod.h"
#include <CoreMinimal.h>
#include <Kismet/BlueprintFunctionLibrary.h>
#include <UObject/TextProperty.h>
#include "DaeTestAssertBlueprintFunctionLibrary.generated.h"

class UImage;
class URichTextBlock;
class UTextBlock;
class UUserWidget;

class ADaeTestTriggerBox;

/** Utility functions for asserting state in automated tests. */
UCLASS()
class DAEDALICTESTAUTOMATIONPLUGIN_API UDaeTestAssertBlueprintFunctionLibrary
    : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Finishes the current test as failure. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertFail(const FString& What, UObject* Context = nullptr);

    /** Expects the specified value to be true. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertTrue(bool bValue, const FString& What, UObject* Context = nullptr);

    /** Expects the specified value to be false. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertFalse(bool bValue, const FString& What, UObject* Context = nullptr);

    /** Expects the specified object to be valid. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertValid(UObject* Object, const FString& What, UObject* Context = nullptr);

    /** Expects the specified object not to be valid. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertInvalid(UObject* Object, const FString& What, UObject* Context = nullptr);

    /** Expects the specified trigger box to be triggered. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertWasTriggered(ADaeTestTriggerBox* TestTriggerBox, UObject* Context = nullptr);

    /** Expects the specified trigger box not to be triggered. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertWasNotTriggered(ADaeTestTriggerBox* TestTriggerBox,
                                      UObject* Context = nullptr);

    /** Expects the specified bytes to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Byte)"))
    static void AssertEqualByte(uint8 Actual, uint8 Expected, const FString& What,
                                UObject* Context = nullptr);

    /** Expects the specified bytes not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Byte)"))
    static void AssertNotEqualByte(uint8 Actual, uint8 Unexpected, const FString& What,
                                   UObject* Context = nullptr);

    /** Compares the specified bytes for order. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Compare (Byte)"))
    static void AssertCompareByte(uint8 First, EDaeTestComparisonMethod ShouldBe, uint8 Second,
                                  const FString& What, UObject* Context = nullptr);

    /** Expects the specified 32-bit integers to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Integer)"))
    static void AssertEqualInt32(int32 Actual, int32 Expected, const FString& What,
                                 UObject* Context = nullptr);

    /** Expects the specified 32-bit integers not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Integer)"))
    static void AssertNotEqualInt32(int32 Actual, int32 Unexpected, const FString& What,
                                    UObject* Context = nullptr);

    /** Compares the specified 32-bit integers for order. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Compare (Integer)"))
    static void AssertCompareInt32(int32 First, EDaeTestComparisonMethod ShouldBe, int32 Second,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified 64-bit integers to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Integer64)"))
    static void AssertEqualInt64(int64 Actual, int64 Expected, const FString& What,
                                 UObject* Context = nullptr);

    /** Expects the specified 64-bit integers not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Integer64)"))
    static void AssertNotEqualInt64(int64 Actual, int64 Unexpected, const FString& What,
                                    UObject* Context = nullptr);

    /** Compares the specified 64-bit integers for order. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Compare (Integer64)"))
    static void AssertCompareInt64(int64 First, EDaeTestComparisonMethod ShouldBe, int64 Second,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified floats to be (nearly) equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Float)"))
    static void AssertEqualFloat(float Actual, float Expected, const FString& What,
                                 UObject* Context = nullptr);

    /** Expects the specified floats not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Float)"))
    static void AssertNotEqualFloat(float Actual, float Unexpected, const FString& What,
                                    UObject* Context = nullptr);

    /** Compares the specified floats for order. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Compare (Float)"))
    static void AssertCompareFloat(float First, EDaeTestComparisonMethod ShouldBe, float Second,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified names to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Name)"))
    static void AssertEqualName(const FName& Actual, const FName& Expected, bool bIgnoreCase,
                                const FString& What, UObject* Context = nullptr);

    /** Expects the specified names not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Name)"))
    static void AssertNotEqualName(const FName& Actual, const FName& Unexpected, bool bIgnoreCase,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified strings to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (String)"))
    static void AssertEqualString(const FString& Actual, const FString& Expected, bool bIgnoreCase,
                                  const FString& What, UObject* Context = nullptr);

    /** Expects the specified strings not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (String)"))
    static void AssertNotEqualString(const FString& Actual, const FString& Unexpected,
                                     bool bIgnoreCase, const FString& What,
                                     UObject* Context = nullptr);

    /** Expects the specified texts to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Text)"))
    static void AssertEqualText(const FText& Actual, const FText& Expected, bool bIgnoreCase,
                                const FString& What, UObject* Context = nullptr);

    /** Expects the specified texts not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Text)"))
    static void AssertNotEqualText(const FText& Actual, const FText& Unexpected, bool bIgnoreCase,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified vectors to be (nearly) equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Vector)"))
    static void AssertEqualVector(const FVector& Actual, const FVector& Expected,
                                  const FString& What, UObject* Context = nullptr);

    /** Expects the specified vectors not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Vector)"))
    static void AssertNotEqualVector(const FVector& Actual, const FVector& Unexpected,
                                     const FString& What, UObject* Context = nullptr);

    /** Expects the specified rotators to be (nearly) equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Rotator)"))
    static void AssertEqualRotator(const FRotator& Actual, const FRotator& Expected,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects the specified rotators not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Rotator)"))
    static void AssertNotEqualRotator(const FRotator& Actual, const FRotator& Unexpected,
                                      const FString& What, UObject* Context = nullptr);

    /** Expects the specified transforms to be (nearly) equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Equal (Transform)"))
    static void AssertEqualTransform(const FTransform& Actual, const FTransform& Expected,
                                     const FString& What, UObject* Context = nullptr);

    /** Expects the specified transforms not to be equal. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not Equal (Transform)"))
    static void AssertNotEqualTransform(const FTransform& Actual, const FTransform& Unexpected,
                                        const FString& What, UObject* Context = nullptr);

    /** Expects Value to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert In Range (Byte)"))
    static void AssertInRangeByte(uint8 Value, uint8 MinInclusive, uint8 MaxInclusive,
                                  const FString& What, UObject* Context = nullptr);

    /** Expects Value not to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not In Range (Byte)"))
    static void AssertNotInRangeByte(uint8 Value, uint8 MinInclusive, uint8 MaxInclusive,
                                     const FString& What, UObject* Context = nullptr);

    /** Expects Value to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert In Range (Integer)"))
    static void AssertInRangeInt32(int32 Value, int32 MinInclusive, int32 MaxInclusive,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects Value not to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not In Range (Integer)"))
    static void AssertNotInRangeInt32(int32 Value, int32 MinInclusive, int32 MaxInclusive,
                                      const FString& What, UObject* Context = nullptr);

    /** Expects Value to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert In Range (Integer64)"))
    static void AssertInRangeInt64(int64 Value, int64 MinInclusive, int64 MaxInclusive,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects Value not to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not In Range (Integer64)"))
    static void AssertNotInRangeInt64(int64 Value, int64 MinInclusive, int64 MaxInclusive,
                                      const FString& What, UObject* Context = nullptr);

    /** Expects Value to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert In Range (Float)"))
    static void AssertInRangeFloat(float Value, float MinInclusive, float MaxInclusive,
                                   const FString& What, UObject* Context = nullptr);

    /** Expects Value not to be between MinInclusive and MaxInclusive. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context",
                                         DisplayName = "Assert Not In Range (Float)"))
    static void AssertNotInRangeFloat(float Value, float MinInclusive, float MaxInclusive,
                                      const FString& What, UObject* Context = nullptr);

    /** Expects the specified widget to be valid and visible (e.g. added to viewport, not hidden or collapsed). */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertWidgetIsVisible(UUserWidget* Widget, const FString& What,
                                      UObject* Context = nullptr);

    /** Expects the specified text not to be empty. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertTextIsSet(UTextBlock* TextBlock, const FString& What,
                                UObject* Context = nullptr);

    /** Expects the specified rich text not to be empty. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertRichTextIsSet(URichTextBlock* RichTextBlock, const FString& What,
                                    UObject* Context = nullptr);

    /** Expects the specified image to be set up to use a texture or material. */
    UFUNCTION(BlueprintCallable, meta = (HidePin = "Context", DefaultToSelf = "Context"))
    static void AssertImageIsSet(UImage* Image, const FString& What, UObject* Context = nullptr);

private:
    static const FString ErrorMessageFormatEqual;
    static const FString ErrorMessageFormatNotEqual;
    static const FString ErrorMessageFormatLessThan;
    static const FString ErrorMessageFormatLessThanOrEqualTo;
    static const FString ErrorMessageFormatGreaterThan;
    static const FString ErrorMessageFormatGreaterThanOrEqualTo;
    static const FString ErrorMessageFormatInRange;
    static const FString ErrorMessageFormatNotInRange;

    static void OnTestFailed(UObject* Context, const FString& Message);

    template<typename T>
    static void AssertEqual(UObject* Context, const FString& What, T Actual, T Expected)
    {
        if (Actual != Expected)
        {
            FString Message = FString::Format(*ErrorMessageFormatEqual, {What, Expected, Actual});
            OnTestFailed(Context, Message);
        }
    }

    template<typename T>
    static void AssertNotEqual(UObject* Context, const FString& What, T Actual, T Unexpected)
    {
        if (Actual == Unexpected)
        {
            FString Message = FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected});
            OnTestFailed(Context, Message);
        }
    }

    template<typename T>
    static void AssertCompare(UObject* Context, const FString& What, T First,
                              EDaeTestComparisonMethod ShouldBe, T Second)
    {
        bool bFulfilled = false;

        switch (ShouldBe)
        {
            case EDaeTestComparisonMethod::LessThan:
                bFulfilled = First < Second;
                break;

            case EDaeTestComparisonMethod::LessThanOrEqualTo:
                bFulfilled = First <= Second;
                break;

            case EDaeTestComparisonMethod::GreaterThanOrEqualTo:
                bFulfilled = First >= Second;
                break;

            case EDaeTestComparisonMethod::GreaterThan:
                bFulfilled = First > Second;
                break;
        }

        if (bFulfilled)
        {
            return;
        }

        FString FormatString;

        switch (ShouldBe)
        {
            case EDaeTestComparisonMethod::LessThan:
                FormatString = ErrorMessageFormatLessThan;
                break;

            case EDaeTestComparisonMethod::LessThanOrEqualTo:
                FormatString = ErrorMessageFormatLessThanOrEqualTo;
                break;

            case EDaeTestComparisonMethod::GreaterThanOrEqualTo:
                FormatString = ErrorMessageFormatGreaterThanOrEqualTo;
                break;

            case EDaeTestComparisonMethod::GreaterThan:
                FormatString = ErrorMessageFormatGreaterThan;
                break;
        }

        FString Message = FString::Format(*FormatString, {What, First, Second});
        OnTestFailed(Context, Message);
    }
};
