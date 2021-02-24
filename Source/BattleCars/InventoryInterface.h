// Cory

#pragma once

#include "UObject/Interface.h"
#include "InventoryInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BATTLECARS_API IInventoryInterface
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.


public:

	//virtual const TMap<UItem*, ItemData>& GetInventoryDataMap() const = 0;



};
