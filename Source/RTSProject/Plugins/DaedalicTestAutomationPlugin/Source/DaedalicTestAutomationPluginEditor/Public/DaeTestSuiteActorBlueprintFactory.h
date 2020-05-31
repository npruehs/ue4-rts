#pragma once

#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include "DaeTestSuiteActorBlueprintFactory.generated.h"

/** Factory for creating new test suite actor blueprints with special initial blueprint graphs. */
UCLASS(HideCategories = Object, MinimalAPI)
class UDaeTestSuiteActorBlueprintFactory : public UFactory
{
    GENERATED_BODY()

public:
    UDaeTestSuiteActorBlueprintFactory(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~ Begin UFactory Interface
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
                                      EObjectFlags Flags, UObject* Context,
                                      FFeedbackContext* Warn) override;
    //~ Begin UFactory Interface
};
