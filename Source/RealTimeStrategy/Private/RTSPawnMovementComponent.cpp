#include "RTSPawnMovementComponent.h"


URTSPawnMovementComponent::URTSPawnMovementComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	// Set reasonable default values.
	bUpdateRotation = true;
}

void URTSPawnMovementComponent::UpdateComponentVelocity()
{
	Super::UpdateComponentVelocity();

	if (bUpdateRotation && !Velocity.IsNearlyZero())
	{
		MoveUpdatedComponent(FVector::ZeroVector, Velocity.Rotation(), false);
	}
}
