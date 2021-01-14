#pragma once

#include "CoreMinimal.h"
#include "Struct_Tile.generated.h"



USTRUCT(BlueprintType)
struct FHHM_TileData_Base {		//For passing basic information to TileData on initialize
	GENERATED_USTRUCT_BODY()
		
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileDataBase)
		float				HP_Max = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileDataBase)
		bool				IsPassable = true;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileDataBase)
		FString				RelativeID_Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TileDataBase)
		int32				RelativeID;*/
};

USTRUCT(BlueprintType)
struct FHHM_TileData {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		int32				ID = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		int32				SubID = 0; //AKA RenderID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		class AHHM_Tile*	Tile = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		int32				Index_Horizontal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		int32				Index_Vertical = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		float				HP_Max = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		float				HP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileData")
		bool				IsPassable = true;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TileInfo")
		bool				IsNeedToRender = true;*/

public:
	FHHM_TileData() {

	}

	FHHM_TileData(class AHHM_Tile* _pTile, FHHM_TileData_Base& _tileData_Base) {
		//ID = _pTile->Get_TileID();
		Tile = _pTile;
				
		HP_Max = _tileData_Base.HP_Max;
		HP = HP_Max;

		IsPassable = _tileData_Base.IsPassable;

	}
};

USTRUCT(BlueprintType)
struct FHHM_TileMovementInfo {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInfo")
		bool		Passable = false;

};