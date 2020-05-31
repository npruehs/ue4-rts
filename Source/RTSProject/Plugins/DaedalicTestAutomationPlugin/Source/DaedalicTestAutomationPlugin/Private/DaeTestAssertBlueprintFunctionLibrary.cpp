#include "DaeTestAssertBlueprintFunctionLibrary.h"
#include "DaeTestActor.h"
#include "DaeTestLogCategory.h"
#include "DaeTestTriggerBox.h"
#include <Blueprint/UserWidget.h>
#include <Components/Image.h>
#include <Components/PanelWidget.h>
#include <Components/RichTextBlock.h>
#include <Components/TextBlock.h>
#include <Kismet/KismetMathLibrary.h>
#include <Styling/SlateBrush.h>

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatEqual =
    TEXT("Assertion failed - {0} - Expected: {1}, but was: {2}");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatNotEqual =
    TEXT("Assertion failed - {0} - Was {1}, but should not be.");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatLessThan =
    TEXT("Assertion failed - {0} - Expected: {1} < {2}, but was not.");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatLessThanOrEqualTo =
    TEXT("Assertion failed - {0} - Expected: {1} <= {2}, but was not.");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatGreaterThan =
    TEXT("Assertion failed - {0} - Expected: {1} > {2}, but was not.");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatGreaterThanOrEqualTo =
    TEXT("Assertion failed - {0} - Expected: {1} >= {2}, but was not.");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatInRange =
    TEXT("Assertion failed - {0} - Expected: between {1} and {2}, but was: {3}");

const FString UDaeTestAssertBlueprintFunctionLibrary::ErrorMessageFormatNotInRange =
    TEXT("Assertion failed - {0} - Expected: not between {1} and {2}, but was: {3}");

void UDaeTestAssertBlueprintFunctionLibrary::AssertFail(const FString& What,
                                                        UObject* Context /*= nullptr*/)
{
    OnTestFailed(Context, What);
}
void UDaeTestAssertBlueprintFunctionLibrary::AssertTrue(bool bValue, const FString& What,
                                                        UObject* Context /*= nullptr*/)
{
    if (!bValue)
    {
        FString Message =
            FString::Format(*ErrorMessageFormatEqual, {What, TEXT("True"), TEXT("False")});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertFalse(bool bValue, const FString& What,
                                                         UObject* Context /*= nullptr*/)
{
    if (bValue)
    {
        FString Message =
            FString::Format(*ErrorMessageFormatEqual, {What, TEXT("False"), TEXT("True")});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertValid(UObject* Object, const FString& What,
                                                         UObject* Context /*= nullptr*/)
{
    if (!IsValid(Object))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Expected: valid, but was: invalid"),
                            {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertInvalid(UObject* Object, const FString& What,
                                                           UObject* Context /*= nullptr*/)
{
    if (IsValid(Object))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Expected: invalid, but was: valid"),
                            {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertWasTriggered(ADaeTestTriggerBox* TestTriggerBox,
                                                                UObject* Context /*= nullptr*/)
{
    if (!IsValid(TestTriggerBox))
    {
        OnTestFailed(Context, TEXT("Invalid test trigger box in assertion"));
        return;
    }

    if (!TestTriggerBox->WasTriggered())
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - Trigger box {0} wasn't triggered"),
                            {TestTriggerBox->GetName()});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertWasNotTriggered(
    ADaeTestTriggerBox* TestTriggerBox, UObject* Context /*= nullptr*/)
{
    if (!IsValid(TestTriggerBox))
    {
        OnTestFailed(Context, TEXT("Invalid test trigger box in assertion"));
        return;
    }

    if (TestTriggerBox->WasTriggered())
    {
        FString Message = FString::Format(TEXT("Assertion failed - Trigger box {0} was triggered"),
                                          {TestTriggerBox->GetName()});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualByte(uint8 Actual, uint8 Expected,
                                                             const FString& What,
                                                             UObject* Context /*= nullptr*/)
{
    AssertEqual(Context, What, Actual, Expected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualByte(uint8 Actual, uint8 Unexpected,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    AssertNotEqual(Context, What, Actual, Unexpected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertCompareByte(uint8 First,
                                                               EDaeTestComparisonMethod ShouldBe,
                                                               uint8 Second, const FString& What,
                                                               UObject* Context /*= nullptr*/)
{
    AssertCompare(Context, What, First, ShouldBe, Second);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualInt32(int32 Actual, int32 Expected,
                                                              const FString& What,
                                                              UObject* Context /*= nullptr*/)
{
    AssertEqual(Context, What, Actual, Expected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualInt32(int32 Actual, int32 Unexpected,
                                                                 const FString& What,
                                                                 UObject* Context /*= nullptr*/)
{
    AssertNotEqual(Context, What, Actual, Unexpected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertCompareInt32(int32 First,
                                                                EDaeTestComparisonMethod ShouldBe,
                                                                int32 Second, const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    AssertCompare(Context, What, First, ShouldBe, Second);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualInt64(int64 Actual, int64 Expected,
                                                              const FString& What,
                                                              UObject* Context /*= nullptr*/)
{
    AssertEqual(Context, What, Actual, Expected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualInt64(int64 Actual, int64 Unexpected,
                                                                 const FString& What,
                                                                 UObject* Context /*= nullptr*/)
{
    AssertNotEqual(Context, What, Actual, Unexpected);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertCompareInt64(int64 First,
                                                                EDaeTestComparisonMethod ShouldBe,
                                                                int64 Second, const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    AssertCompare(Context, What, First, ShouldBe, Second);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualFloat(float Actual, float Expected,
                                                              const FString& What,
                                                              UObject* Context /*= nullptr*/)
{
    if (!FMath::IsNearlyEqual(Actual, Expected))
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual, {What, Expected, Actual});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualFloat(float Actual, float Unexpected,
                                                                 const FString& What,
                                                                 UObject* Context /*= nullptr*/)
{
    if (FMath::IsNearlyEqual(Actual, Unexpected))
    {
        FString Message = FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertCompareFloat(float First,
                                                                EDaeTestComparisonMethod ShouldBe,
                                                                float Second, const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    AssertCompare(Context, What, First, ShouldBe, Second);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualName(const FName& Actual,
                                                             const FName& Expected,
                                                             bool bIgnoreCase, const FString& What,
                                                             UObject* Context /*= nullptr*/)
{
    bool bEquals = bIgnoreCase ? Actual.IsEqual(Expected, ENameCase::IgnoreCase)
                               : Actual.IsEqual(Expected, ENameCase::CaseSensitive);

    if (!bEquals)
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual,
                                          {What, Expected.ToString(), Actual.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualName(const FName& Actual,
                                                                const FName& Unexpected,
                                                                bool bIgnoreCase,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    bool bEquals = bIgnoreCase ? Actual.IsEqual(Unexpected, ENameCase::IgnoreCase)
                               : Actual.IsEqual(Unexpected, ENameCase::CaseSensitive);

    if (bEquals)
    {
        FString Message =
            FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualString(const FString& Actual,
                                                               const FString& Expected,
                                                               bool bIgnoreCase,
                                                               const FString& What,
                                                               UObject* Context /*= nullptr*/)
{
    ESearchCase::Type SearchCase = bIgnoreCase ? ESearchCase::IgnoreCase
                                               : ESearchCase::CaseSensitive;

    if (!Actual.Equals(Expected, SearchCase))
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual, {What, Expected, Actual});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualString(const FString& Actual,
                                                                  const FString& Unexpected,
                                                                  bool bIgnoreCase,
                                                                  const FString& What,
                                                                  UObject* Context /*= nullptr*/)
{
    ESearchCase::Type SearchCase = bIgnoreCase ? ESearchCase::IgnoreCase
                                               : ESearchCase::CaseSensitive;

    if (Actual.Equals(Unexpected, SearchCase))
    {
        FString Message = FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualText(const FText& Actual,
                                                             const FText& Expected,
                                                             bool bIgnoreCase, const FString& What,
                                                             UObject* Context /*= nullptr*/)
{
    bool bEquals = bIgnoreCase ? Actual.EqualToCaseIgnored(Expected) : Actual.EqualTo(Expected);

    if (!bEquals)
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual,
                                          {What, Expected.ToString(), Actual.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualText(const FText& Actual,
                                                                const FText& Unexpected,
                                                                bool bIgnoreCase,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    bool bEquals = bIgnoreCase ? Actual.EqualToCaseIgnored(Unexpected) : Actual.EqualTo(Unexpected);

    if (bEquals)
    {
        FString Message =
            FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualVector(const FVector& Actual,
                                                               const FVector& Expected,
                                                               const FString& What,
                                                               UObject* Context /*= nullptr*/)
{
    if (!Actual.Equals(Expected))
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual,
                                          {What, Expected.ToString(), Actual.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualVector(const FVector& Actual,
                                                                  const FVector& Unexpected,
                                                                  const FString& What,
                                                                  UObject* Context /*= nullptr*/)
{
    if (Actual.Equals(Unexpected))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualRotator(const FRotator& Actual,
                                                                const FRotator& Expected,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    if (!Actual.Equals(Expected))
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual,
                                          {What, Expected.ToString(), Actual.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualRotator(const FRotator& Actual,
                                                                   const FRotator& Unexpected,
                                                                   const FString& What,
                                                                   UObject* Context /*= nullptr*/)
{
    if (Actual.Equals(Unexpected))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertEqualTransform(const FTransform& Actual,
                                                                  const FTransform& Expected,
                                                                  const FString& What,
                                                                  UObject* Context /*= nullptr*/)
{
    if (!Actual.Equals(Expected))
    {
        FString Message = FString::Format(*ErrorMessageFormatEqual,
                                          {What, Expected.ToString(), Actual.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotEqualTransform(const FTransform& Actual,
                                                                     const FTransform& Unexpected,
                                                                     const FString& What,
                                                                     UObject* Context /*= nullptr*/)
{
    if (Actual.Equals(Unexpected))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatNotEqual, {What, Unexpected.ToString()});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertInRangeByte(uint8 Value, uint8 MinInclusive,
                                                               uint8 MaxInclusive,
                                                               const FString& What,
                                                               UObject* Context /*= nullptr*/)
{
    AssertInRangeInt32(Value, MinInclusive, MaxInclusive, What, Context);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotInRangeByte(uint8 Value, uint8 MinInclusive,
                                                                  uint8 MaxInclusive,
                                                                  const FString& What,
                                                                  UObject* Context /*= nullptr*/)
{
    AssertNotInRangeInt32(Value, MinInclusive, MaxInclusive, What, Context);
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertInRangeInt32(int32 Value, int32 MinInclusive,
                                                                int32 MaxInclusive,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    if (!UKismetMathLibrary::InRange_IntInt(Value, MinInclusive, MaxInclusive))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatInRange, {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotInRangeInt32(int32 Value, int32 MinInclusive,
                                                                   int32 MaxInclusive,
                                                                   const FString& What,
                                                                   UObject* Context /*= nullptr*/)
{
    if (UKismetMathLibrary::InRange_IntInt(Value, MinInclusive, MaxInclusive))
    {
        FString Message = FString::Format(*ErrorMessageFormatNotInRange,
                                          {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertInRangeInt64(int64 Value, int64 MinInclusive,
                                                                int64 MaxInclusive,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    if (!UKismetMathLibrary::InRange_Int64Int64(Value, MinInclusive, MaxInclusive))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatInRange, {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotInRangeInt64(int64 Value, int64 MinInclusive,
                                                                   int64 MaxInclusive,
                                                                   const FString& What,
                                                                   UObject* Context /*= nullptr*/)
{
    if (UKismetMathLibrary::InRange_Int64Int64(Value, MinInclusive, MaxInclusive))
    {
        FString Message = FString::Format(*ErrorMessageFormatNotInRange,
                                          {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertInRangeFloat(float Value, float MinInclusive,
                                                                float MaxInclusive,
                                                                const FString& What,
                                                                UObject* Context /*= nullptr*/)
{
    if (!UKismetMathLibrary::InRange_FloatFloat(Value, MinInclusive, MaxInclusive))
    {
        FString Message =
            FString::Format(*ErrorMessageFormatInRange, {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertNotInRangeFloat(float Value, float MinInclusive,
                                                                   float MaxInclusive,
                                                                   const FString& What,
                                                                   UObject* Context /*= nullptr*/)
{
    if (UKismetMathLibrary::InRange_FloatFloat(Value, MinInclusive, MaxInclusive))
    {
        FString Message = FString::Format(*ErrorMessageFormatNotInRange,
                                          {What, MinInclusive, MaxInclusive, Value});
        OnTestFailed(Context, Message);
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertWidgetIsVisible(UUserWidget* Widget,
                                                                   const FString& What,
                                                                   UObject* Context /*= nullptr*/)
{
    if (!IsValid(Widget))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Widget is not valid"), {What});
        OnTestFailed(Context, Message);
        return;
    }

    if (!Widget->IsInViewport() && !IsValid(Widget->GetParent()))
    {
        FString Message = FString::Format(
            TEXT("Assertion failed - {0} - Widget hasn't been added to the viewport"), {What});
        OnTestFailed(Context, Message);
        return;
    }

    if (!Widget->IsVisible())
    {
        FString Message = FString::Format(TEXT("Assertion failed - {0} - Widget isn't visible, hit "
                                               "test visible or self hit test visible"),
                                          {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertTextIsSet(UTextBlock* TextBlock,
                                                             const FString& What,
                                                             UObject* Context /*= nullptr*/)
{
    if (!IsValid(TextBlock))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Text block is not valid"), {What});
        OnTestFailed(Context, Message);
        return;
    }

    if (TextBlock->GetText().IsEmpty())
    {
        FString Message = FString::Format(TEXT("Assertion failed - {0} - Text is empty"), {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertRichTextIsSet(URichTextBlock* RichTextBlock,
                                                                 const FString& What,
                                                                 UObject* Context /*= nullptr*/)
{
    if (!IsValid(RichTextBlock))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Rich text block is not valid"), {What});
        OnTestFailed(Context, Message);
        return;
    }

    if (RichTextBlock->GetText().IsEmpty())
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Rich text is empty"), {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::AssertImageIsSet(UImage* Image, const FString& What,
                                                              UObject* Context /*= nullptr*/)
{
    if (!IsValid(Image))
    {
        FString Message =
            FString::Format(TEXT("Assertion failed - {0} - Image is not valid"), {What});
        OnTestFailed(Context, Message);
        return;
    }

    if (!IsValid(Image->Brush.GetResourceObject()))
    {
        FString Message = FString::Format(TEXT("Assertion failed - {0} - Image brush has no "
                                               "resource object (e.g. texture or material)"),
                                          {What});
        OnTestFailed(Context, Message);
        return;
    }
}

void UDaeTestAssertBlueprintFunctionLibrary::OnTestFailed(UObject* Context, const FString& Message)
{
    ADaeTestActor* TestActor = Cast<ADaeTestActor>(Context);

    if (IsValid(TestActor))
    {
        TestActor->NotifyOnTestFailed(Message);
    }
    else
    {
        UE_LOG(LogDaeTest, Error, TEXT("%s"), *Message);
    }
}
