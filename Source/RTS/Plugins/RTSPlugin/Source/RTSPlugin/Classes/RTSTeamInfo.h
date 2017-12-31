#pragma once

#include "RTSPluginPCH.h"

#include "GameFramework/Info.h"

#include "RTSTeamInfo.generated.h"


/**
* Team that consists of multiple players.
*/
UCLASS()
class RTSPLUGIN_API ARTSTeamInfo : public AInfo
{
	GENERATED_BODY()

	ARTSTeamInfo();

public:
	/** Index of this team. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = ReceivedTeamIndex, Category = "Team")
	uint8 TeamIndex;

	/** Adds the specified player to this team. */
	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual void AddToTeam(AController* Player);

	/** Removes the specified player from this team. */
	UFUNCTION(BlueprintCallable, Category = "Team")
	virtual void RemoveFromTeam(AController* Player);

	/** Gets all players belonging to this  team. */
	UFUNCTION()
	TArray<AController*> GetTeamMembers();


protected:
	UFUNCTION()
	virtual void ReceivedTeamIndex();

private:
	/** Players on this team. */
	TArray<AController*> TeamMembers;
};
