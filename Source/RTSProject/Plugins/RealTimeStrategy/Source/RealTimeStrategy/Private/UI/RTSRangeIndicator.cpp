#include "UI/RTSRangeIndicator.h"


float ARTSRangeIndicator::GetRange() const
{
    return Range;
}

void ARTSRangeIndicator::SetRange(float InRange)
{
    Range = InRange;

    NotifyOnRangeChanged(Range);
}

void ARTSRangeIndicator::NotifyOnRangeChanged(float NewRange)
{
    ReceiveOnRangeChanged(NewRange);
}
