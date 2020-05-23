#include "DaeTestAssumeBlueprintFunctionLibrary.h"
#include "DaeTestActor.h"
#include "DaeTestLogCategory.h"

void UDaeTestAssumeBlueprintFunctionLibrary::AssumeTrue(bool bValue, const FString& What,
                                                        UObject* Context /*= nullptr*/)
{
    if (!bValue)
    {
        FString Message =
            FString::Format(TEXT("Assumption failed - {0} - Expected: True, but was: False"),
                            {What});
        OnTestSkipped(Context, Message);
    }
}

void UDaeTestAssumeBlueprintFunctionLibrary::AssumeFalse(bool bValue, const FString& What,
                                                         UObject* Context /*= nullptr*/)
{
    if (bValue)
    {
        FString Message =
            FString::Format(TEXT("Assumption failed - {0} - Expected: False, but was: True"),
                            {What});
        OnTestSkipped(Context, Message);
    }
}

void UDaeTestAssumeBlueprintFunctionLibrary::OnTestSkipped(UObject* Context, const FString& Message)
{
    ADaeTestActor* TestActor = Cast<ADaeTestActor>(Context);

    if (IsValid(TestActor))
    {
        TestActor->NotifyOnTestSkipped(Message);
    }
    else
    {
        UE_LOG(LogDaeTest, Log, TEXT("%s"), *Message);
    }
}
