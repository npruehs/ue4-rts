#include "RTSPluginPCH.h"
#include "RTSFloatingCombatTextComponent.h"


URTSFloatingCombatTextComponent::URTSFloatingCombatTextComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
}

void URTSFloatingCombatTextComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    for (int32 Index = Texts.Num() - 1; Index >= 0; --Index)
    {
        FRTSFloatingCombatTextData& TextData = Texts[Index];
        TextData.RemainingLifetime -= DeltaTime;

        if (TextData.RemainingLifetime <= 0.0f)
        {
            Texts.RemoveAt(Index);
        }
    }
}

void URTSFloatingCombatTextComponent::AddText(const FString& Text, const FLinearColor& Color, float Scale, float Lifetime)
{
    FRTSFloatingCombatTextData TextData;
    TextData.Text = Text;
    TextData.Color = Color;
    TextData.Scale = Scale;
    TextData.Lifetime = Lifetime;
    TextData.RemainingLifetime = Lifetime;

    Texts.Add(TextData);
}
