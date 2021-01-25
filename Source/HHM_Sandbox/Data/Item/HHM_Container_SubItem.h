#pragma once

#include "EngineMinimal.h"
#include "HHM_Container_SubItem.generated.h"

USTRUCT()
struct FHHM_Container_SubItem {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY()
		TMap<int32, class UHHM_Item*>	Container_SubItem;

public:
	FHHM_Container_SubItem() {
		Container_SubItem.Empty();
	}
};