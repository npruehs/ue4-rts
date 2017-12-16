#include "RTSPluginPCH.h"
#include "RTSSelectableComponent.h"


void URTSSelectableComponent::SelectActor()
{
	if (bSelected)
	{
		return;
	}

	bSelected = true;

	// Notify listeners.
	OnSelected.Broadcast();
}

void URTSSelectableComponent::DeselectActor()
{
	if (!bSelected)
	{
		return;
	}

	bSelected = false;

	// Notify listeners.
	OnDeselected.Broadcast();
}

bool URTSSelectableComponent::IsSelected()
{
	return bSelected;
}
