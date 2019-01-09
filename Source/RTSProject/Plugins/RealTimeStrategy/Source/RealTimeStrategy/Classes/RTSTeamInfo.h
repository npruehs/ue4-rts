#pragma once

#include "RealTimeStrategyPCH.h"

#include "GameFramework/Info.h"

#include "RTSTeamInfo.generated.h"


/**
* Team that consists of multiple players.
*/
UCLASS()
class REALTIMESTRATEGY_API ARTSTeamInfo : public AInfo
{
	GENERATED_BODY()

public:
    ARTSTeamInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Adds the specified player to this team. */
	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual void AddToTeam(AController* Player);

	/** Removes the specified player from this team. */
	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual void RemoveFromTeam(AController* Player);

    /** Gets the index of this team. */
    UFUNCTION(BlueprintPure)
    uint8 GetTeamIndex() const;

	/** Gets all players belonging to this  team. */
	UFUNCTION(BlueprintPure)
	TArray<AController*> GetTeamMembers() const;

    /** Sets the index of this team. */
    void SetTeamIndex(uint8 InTeamIndex);


private:
    /** Index of this team. */
    UPROPERTY(ReplicatedUsing = ReceivedTeamIndex)
    uint8 TeamIndex;

	/** Players on this team. */
    UPROPERTY()
	TArray<AController*> TeamMembers;

    UFUNCTION()
    void ReceivedTeamIndex();
};
