#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"

#include "RTSPlayerState.generated.h"


class AActor;

class ARTSTeamInfo;


/**
* Player state with RTS features, such as teams.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    static const uint8 PLAYER_INDEX_NONE;

    ARTSPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Gets the index of the player. */
    UFUNCTION(BlueprintPure)
    uint8 GetPlayerIndex() const;

    /** Gets the team this player belongs to. */
    UFUNCTION(BlueprintPure)
    ARTSTeamInfo* GetTeam() const;

    /** Sets the index of the player. */
    void SetPlayerIndex(uint8 InPlayerIndex);

    /** Sets the team this player belongs to. */
    void SetTeam(ARTSTeamInfo* InTeam);

	/** Checks whether this player belong to the same team as the specified one. */
	UFUNCTION(BlueprintPure)
	bool IsSameTeamAs(const ARTSPlayerState* Other) const;

    /** Gets the list of actors currently owned by this player. */
    UFUNCTION(BlueprintPure)
    TArray<AActor*> GetOwnActors() const;

    /** Looks for all actors currently owned by this player and caches their references. */
    void DiscoverOwnActors();


	/** Event when this player changed team. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

    /** Event when this player is now owning or no longer owning the specified actor. */
    virtual void NotifyOnActorOwnerChanged(AActor* Actor, ARTSPlayerState* OldOwner, ARTSPlayerState* NewOwner);

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);

private:
    /** Index of the player. */
    UPROPERTY(Replicated)
    uint8 PlayerIndex;

    /** Team this player belongs to. */
    UPROPERTY(ReplicatedUsing = OnTeamChanged)
    ARTSTeamInfo* Team;

    /** Actors this player owns. */
    UPROPERTY()
    TArray<AActor*> OwnActors;


    UFUNCTION()
    void OnTeamChanged();
};
