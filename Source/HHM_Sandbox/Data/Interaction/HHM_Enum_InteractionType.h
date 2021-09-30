#pragma once

#include "CoreMinimal.h"
#include "ObjectMacros.h"



UENUM(BlueprintType)
enum class EHHM_InteractionType : uint8
{
	Type_Interaction_SelfToEntity		UMETA(DisplayName = "Type_InteractionToEntity"),
	Type_Interaction_SelfToTile			UMETA(DisplayName = "Type_InteractionToTile"),
	Type_Interaction_SelfToLocation		UMETA(DisplayName = "Type_InteractionToLocation"),
	Type_Interaction_SelfToSelf			UMETA(DisplayName = "Type_InteractionToSelf"),
	Type_Interaction_EntityToSelf		UMETA(DisplayName = "Type_InteractionToSelf"),
	Type_Interaction_TileToSelf			UMETA(DisplayName = "Type_InteractionToSelf"),

	Type_Item_Get_From_Entity		UMETA(DisplayName = "Type_GetItemFromEntity"),
	Type_Item_Get_From_Tile			UMETA(DisplayName = "Type_GetItemFromTile"),
	Type_Item_Give_To_Entity		UMETA(DisplayName = "Type_GiveItemToEntity"),
	Type_Item_Give_To_Tile			UMETA(DisplayName = "Type_GiveItemToTile"),
	Type_Talk_SelfToTarget			UMETA(DisplayName = "Type_TalkTo"),
	Type_Talk_TargetToSelf			UMETA(DisplayName = "Type_TalkFrom"),
	Type_End						UMETA(DisplayName = "Type_End")
};