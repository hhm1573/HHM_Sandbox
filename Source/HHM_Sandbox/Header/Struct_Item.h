#pragma once

#include "CoreMinimal.h"
#include "Struct_Item.generated.h"

USTRUCT(BlueprintType)
struct FHHM_ItemData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int32				ID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int32				SubID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		class UHHM_Item*	Item = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemData")
		int32				Remain_UseCount = 0;

public:
	FHHM_ItemData() {

	}

	FHHM_ItemData(class UHHM_Item* _item, int32 _ID, int32 _subID) {	// HHM Note : Tie Input arguments when it gets to large. (Refer FHHM_TileData_Base)
		ID = _ID;
		SubID = _subID;
		Item = _item;
	}
};