#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/PlayerState.h"

#include "RTSPlayerState.generated.h"


class ARTSTeamInfo;


/**
* Player state with RTS features, such as teams.
*/
UCLASS()
class RTSPLUGIN_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	/** Team this player belongs to. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnTeamChanged, Category = "Team")
	ARTSTeamInfo* Team;


	/** Checks whether this player belong to the same team as the specified one. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAs(ARTSPlayerState* Other);


	UFUNCTION()
	void OnTeamChanged();

	/** Event when this player changed team. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);
};
