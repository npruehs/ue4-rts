#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/PlayerState.h"

#include "RTSPlayerState.generated.h"


class ARTSTeamInfo;


/**
* Player state with RTS features, such as teams.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Gets the team this player belongs to. */
    UFUNCTION(BlueprintPure)
    ARTSTeamInfo* GetTeam() const;

    /** Sets the team this player belongs to. */
    void SetTeam(ARTSTeamInfo* InTeam);

	/** Checks whether this player belong to the same team as the specified one. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAs(ARTSPlayerState* Other) const;


	/** Event when this player changed team. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);

private:
    /** Team this player belongs to. */
    UPROPERTY(ReplicatedUsing = OnTeamChanged)
    ARTSTeamInfo* Team;


    UFUNCTION()
    void OnTeamChanged();
};
