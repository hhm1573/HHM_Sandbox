#pragma once

#include "CoreMinimal.h"

//#include "Data/Item/ItemData/HHM_ItemData.h"

#include "HHM_Container_ItemData.generated.h"



USTRUCT()
struct FHHM_Container_ItemData{

	GENERATED_USTRUCT_BODY();

public:
	UPROPERTY()
		TMap<int32, class UHHM_ItemData*>		Container_ItemData;



public:
	FHHM_Container_ItemData() {
		Container_ItemData.Empty();
	}

};