#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/PlayerState.h"

#include "RTSPlayerState.generated.h"


class ARTSTeamInfo;


/**
* Player state with RTS features, such as teams.
*/
UCLASS()
class ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** Team this player belongs to. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = NotifyOnTeamChanged, Category = Team)
	ARTSTeamInfo* Team;


	/** Checks whether this player belong to the same team as the specified one. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAs(ARTSPlayerState* Other);


	/** Event when this player changed team. */
	UFUNCTION()
	virtual void NotifyOnTeamChanged();

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged();
};