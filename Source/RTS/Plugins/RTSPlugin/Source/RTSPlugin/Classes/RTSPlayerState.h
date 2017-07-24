#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/PlayerState.h"

#include "RTSPlayerState.generated.h"


class ARTSTeamInfo;


UCLASS()
class ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** Team this player belongs to. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = NotifyOnTeamChanged, Category = Team)
	ARTSTeamInfo* Team;


	/** Event when this player changed team. */
	UFUNCTION()
	virtual void NotifyOnTeamChanged();

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged();
};