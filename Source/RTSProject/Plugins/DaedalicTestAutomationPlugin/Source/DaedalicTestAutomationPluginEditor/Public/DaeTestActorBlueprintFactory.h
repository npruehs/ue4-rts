#pragma once

#include <CoreMinimal.h>
#include <Factories/Factory.h>
#include "DaeTestActorBlueprintFactory.generated.h"

/** Factory for creating new test actor blueprints with special initial blueprint graphs. */
UCLASS(HideCategories = Object, MinimalAPI)
class UDaeTestActorBlueprintFactory : public UFactory
{
    GENERATED_BODY()

public:
    UDaeTestActorBlueprintFactory(
        const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    //~ Begin UFactory Interface
    virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name,
                                      EObjectFlags Flags, UObject* Context,
                                      FFeedbackContext* Warn) override;
    //~ Begin UFactory Interface
};
