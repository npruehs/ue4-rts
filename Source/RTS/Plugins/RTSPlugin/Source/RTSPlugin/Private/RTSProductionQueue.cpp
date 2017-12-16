#include "RTSPluginPCH.h"
#include "RTSProductionQueue.h"


TSubclassOf<AActor> FRTSProductionQueue::operator[](int32 Index) const
{
	return Queue[Index];
}

void FRTSProductionQueue::Add(TSubclassOf<AActor> Product)
{
	Queue.Add(Product);
}

int32 FRTSProductionQueue::Num() const
{
	return Queue.Num();
}

void FRTSProductionQueue::RemoveAt(int32 Index)
{
	// Advance all products.
	int32 Slots = Queue.Num();

	for (int32 SlotToUpdate = Index; SlotToUpdate < Slots - 1; ++SlotToUpdate)
	{
		Queue[SlotToUpdate] = Queue[SlotToUpdate + 1];
	}

	Queue.RemoveAt(Slots - 1);
}
