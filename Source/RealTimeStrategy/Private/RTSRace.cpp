// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSRace.h"

TArray<TSubclassOf<AActor>> URTSRace::GetBuildOrder() const
{
	TArray<TSubclassOf<AActor>> Classes;

	if (bIncludeInitialActorsInBuildOrder)
	{
		Classes.Append(InitialActors);
	}

	Classes.Append(BuildOrder);

	return Classes;
}
