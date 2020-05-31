#include "DaeTestActor.h"
#include "DaeTestLogCategory.h"
#include "DaeTestParameterProviderActor.h"

ADaeTestActor::ADaeTestActor(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : Super(ObjectInitializer)
{
    TimeoutInSeconds = 30.0f;
}

void ADaeTestActor::ApplyParameterProviders()
{
    for (int32 Index = 0; Index < ParameterProviders.Num(); ++Index)
    {
        ADaeTestParameterProviderActor* Provider = ParameterProviders[Index];

        if (!IsValid(Provider))
        {
            UE_LOG(LogDaeTest, Error,
                   TEXT("ADaeTestActor::ApplyParameterProviders - %s has invalid parameter "
                        "provider at index %i, skipping."),
                   *GetName(), Index);

            continue;
        }

        TArray<UObject*> AdditionalParameters = Provider->GetParameters();
        Parameters.Append(AdditionalParameters);

        UE_LOG(LogDaeTest, Log,
               TEXT("ADaeTestActor::ApplyParameterProviders - %s appended %i additional parameters "
                    "provided by %s."),
               *GetName(), AdditionalParameters.Num(), *Provider->GetName());
    }
}

void ADaeTestActor::RunTest(UObject* TestParameter)
{
    CurrentParameter = TestParameter;
    bHasResult = false;

    if (!SkipReason.IsEmpty())
    {
        NotifyOnTestSkipped(SkipReason);
        return;
    }

    NotifyOnAssume(CurrentParameter);

    if (bHasResult)
    {
        // This can happen with failed assumptions, for instance.
        return;
    }

    NotifyOnArrange(CurrentParameter);
    NotifyOnAct(CurrentParameter);
}

void ADaeTestActor::FinishAct()
{
    if (bHasResult)
    {
        UE_LOG(LogDaeTest, Warning,
               TEXT("Test %s already has a result. This can happen after a timeout due to delays, "
                    "but if not, make sure not to call FinishAct more than once."),
               *GetName());
        return;
    }

    NotifyOnAssert(CurrentParameter);

    if (!bHasResult)
    {
        NotifyOnTestSuccessful();
    }
}

float ADaeTestActor::GetTimeoutInSeconds() const
{
    return TimeoutInSeconds;
}

TArray<TSoftObjectPtr<UObject>> ADaeTestActor::GetParameters() const
{
    return Parameters;
}

UObject* ADaeTestActor::GetCurrentParameter() const
{
    return CurrentParameter;
}

void ADaeTestActor::NotifyOnTestSuccessful()
{
    if (bHasResult)
    {
        return;
    }

    bHasResult = true;

    OnTestSuccessful.Broadcast(this, CurrentParameter);
}

void ADaeTestActor::NotifyOnTestFailed(const FString& Message)
{
    if (bHasResult)
    {
        return;
    }

    bHasResult = true;

    UE_LOG(LogDaeTest, Error, TEXT("%s"), *Message);

    OnTestFailed.Broadcast(this, CurrentParameter, Message);
}

void ADaeTestActor::NotifyOnTestSkipped(const FString& InSkipReason)
{
    if (bHasResult)
    {
        return;
    }

    bHasResult = true;

    OnTestSkipped.Broadcast(this, CurrentParameter, InSkipReason);
}

void ADaeTestActor::NotifyOnAssume(UObject* Parameter)
{
    ReceiveOnAssume(Parameter);
}

void ADaeTestActor::NotifyOnArrange(UObject* Parameter)
{
    ReceiveOnArrange(Parameter);
}

void ADaeTestActor::NotifyOnAct(UObject* Parameter)
{
    ReceiveOnAct(Parameter);
}

void ADaeTestActor::NotifyOnAssert(UObject* Parameter)
{
    ReceiveOnAssert(Parameter);
}

void ADaeTestActor::ReceiveOnAct_Implementation(UObject* Parameter)
{
    FinishAct();
}
