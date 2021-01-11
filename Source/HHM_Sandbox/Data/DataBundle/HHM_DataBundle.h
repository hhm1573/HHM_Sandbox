#pragma once

#include "CoreMinimal.h"

#include "Header/Enum_Type.h"

#include "HHM_DataBundle.generated.h"

USTRUCT(BlueprintType)
struct FHHM_DataBundle 
{
	GENERATED_USTRUCT_BODY()

public:
	TMap<FString, FString>	DataMap;
	bool					IsValid = false;

public:
	FHHM_DataBundle() {
		DataMap.Empty();
		IsValid = true;
	}

	FHHM_DataBundle(const FString& _serializedData) {
		IsValid = DeSerialization(_serializedData, *this);
	}

public:
	bool Serialization(FString& _serializedDataStorage) {
		return true;
	}

	bool DeSerialization(const FString& _serializedData, FHHM_DataBundle& _deSerializedDataStorage) {
		return true;
	}

	bool Add(const FString& _dataName, EHHM_DataType _dataType, const FString& _dataValue) {

	}

	bool Remove(const FString& _dataPath) {

	}

	bool Has(const FString& _dataPath) {

	}

	bool Set(const FString& _dataPath, const FString& _dataValue) {

	}

	

private:
	//bool SplitString(TArray<FString>& _container_String_Return, const FString& _string_Spliting)	재귀형식으로 진행할것이기에 안에 있는 두번째 이상의 [부터]까지는 무시한다.
	//bool ConstructDataBundle(FHHM_DataBundle& _dataBundle_Return, const FString& _serializedData)
};