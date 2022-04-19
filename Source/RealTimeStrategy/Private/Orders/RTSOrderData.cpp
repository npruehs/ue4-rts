#include "Orders/RTSOrderData.h"

FRTSOrderData::FRTSOrderData()
    : OrderClass(nullptr),
    TargetActor(nullptr),
    TargetLocation(FVector::ZeroVector),
    Index(INDEX_NONE)
{
}
