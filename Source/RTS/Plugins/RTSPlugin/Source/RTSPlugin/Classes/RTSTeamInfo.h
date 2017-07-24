#pragma once

#include "RTSPluginPrivatePCH.h"

#include "GameFramework/Info.h"

#include "RTSTeamInfo.generated.h"


UCLASS()
class ARTSTeamInfo : public AInfo
{
	GENERATED_BODY()

	ARTSTeamInfo();

public:
	/** Index of this team. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = ReceivedTeamIndex, Category = Team)
	uint8 TeamIndex;

	UFUNCTION(BlueprintCallable, Category = Team)
	virtual void AddToTeam(AController* Player);

	UFUNCTION(BlueprintCallable, Category = Team)
	virtual void RemoveFromTeam(AController* Player);

	UFUNCTION()
	TArray<AController*> GetTeamMembers();


protected:
	UFUNCTION()
	virtual void ReceivedTeamIndex();

private:
	/** Players on this team. */
	TArray<AController*> TeamMembers;
};