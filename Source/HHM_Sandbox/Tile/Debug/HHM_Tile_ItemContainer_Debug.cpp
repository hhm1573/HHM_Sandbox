// Fill out your copyright notice in the Description page of Project Settings.


#include "HHM_Tile_ItemContainer_Debug.h"
#include "Base/GameMode/HHM_GameMode_LocalMap.h"
#include "Manager/Item/HHM_Manager_Item.h"



AHHM_Tile_ItemContainer_Debug::AHHM_Tile_ItemContainer_Debug() {
	m_ID = 9999;

	m_Data_Inventory.m_Size_Horizontal = 6;
	m_Data_Inventory.m_Size_Vertical = 6;

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Asset_Material(TEXT("/Game/HHM_Develope/Core/Resource/Mat_Tile_Seamless"));
	if (Asset_Material.Succeeded() == true) {
		static ConstructorHelpers::FObjectFinder<UTexture> Asset_Texture(TEXT("/Game/HHM_Develope/Core/Resource/Texture/Develope/Tile/Tile_Container_Debug"));
		if (Asset_Texture.Succeeded() == true) {

			UMaterialInstanceDynamic* pMaterialInstance = UMaterialInstanceDynamic::Create(Asset_Material.Object, this, TEXT("Tile_Material_Debug_Container"));
			pMaterialInstance->SetTextureParameterValue(TEXT("Param_Texture"), Asset_Texture.Object);
			pMaterialInstance->SetScalarParameterValue(TEXT("Param_TextureSize"), 10.0f);

			m_RenderInfo.Arr_Material.Add(pMaterialInstance);
			m_RenderInfo.Num_Material = 1;
		}
		else {
			//Exception Cant find texture
		}
	}
	else {
		//Exception Cant find material
	}
}

bool AHHM_Tile_ItemContainer_Debug::On_Placed(ALocalMap* _pLocalMap, FHHM_TileData& _tileInfo, AHHM_Entity* _pEntity)
{
	const bool IsSucceed_SuperFunc = Super::On_Placed(_pLocalMap, _tileInfo, _pEntity);
	if (IsSucceed_SuperFunc == false) {
		//Exception
		return false;
	}

	UWorld* pWorld = nullptr;	//HHM Note : 인벤토리 UI와 컨테이너 타일 엔티티와의 상호작용 테스트를 위한 코드부분. 추후 테스트가 끝나면 삭제.
	pWorld = GetWorld();
	if (pWorld) {
		AGameModeBase* pGameMode_Raw = nullptr;
		pGameMode_Raw = pWorld->GetAuthGameMode();
		if (pGameMode_Raw) {
			AHHM_GameMode_LocalMap* pGameMode = nullptr;
			pGameMode = Cast<AHHM_GameMode_LocalMap>(pGameMode_Raw);
			if (pGameMode) {
				AHHM_Manager_Item* pManager_Item = nullptr;
				pManager_Item = pGameMode->Get_Manager_Item();
				if (pManager_Item) {
					UHHM_ItemData* pItemData_Cup = nullptr;
					pItemData_Cup = pManager_Item->Create_Default_ItemData_By_ID(0);
					if (pItemData_Cup) {
						int32 InventoryIndex_Horizontal = FMath::RandRange(0, 3);
						int32 InventoryIndex_Vertical = FMath::RandRange(0, 3);
						int32 InventoryItemID_Temp = 0;

						const bool IsSucceed_ItemAdd = On_Item_Add_At(_pLocalMap, _tileInfo, nullptr, pItemData_Cup, InventoryIndex_Horizontal, InventoryIndex_Vertical);
						if (IsSucceed_ItemAdd == false) {
							//Exception
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}
