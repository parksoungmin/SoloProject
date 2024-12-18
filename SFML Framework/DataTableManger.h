#pragma once

#include "ItemTable.h"
#include "Block1Table.h"
#include "MonsterTable.h"
#include "Stage1MapTable.h"
#include "ParticleTable.h"

class DataTableManger : public Singleton<DataTableManger>
{
	friend class Singleton<DataTableManger>;

private:
	std::unordered_map<DataTable::Types, DataTable*> tableMap;
	//맵 대신 언로더드 맵을 쓰는 이유 3가지
	//1. 배열 접근이 가능하다.
	//2. 데이터를 찾을 때에 0(1) 시간 복잡도를 가져 개이득이다
	//3. 데이터를 삭제하지 않을꺼다. && 중복키를 허용하지 않는다.(되긴함)


public:
	void Init();
	void Release();

	template<typename T>
	T* GetDataTable(DataTable::Types type);

protected:
	DataTableManger() = default;
	virtual ~DataTableManger();
	DataTableManger(const DataTableManger& other) = delete;
	DataTableManger& operator=(const DataTableManger& other) = delete;
};

template<typename T>
inline T* DataTableManger::GetDataTable(DataTable::Types type)
{
	auto find = tableMap.find(type);

	if (find == tableMap.end())
		return nullptr;

	return dynamic_cast<T*>(find->second);
}

#define DATATABLE_MANEGER (DataTableManger::Instance())
#define STRING_TABLE (DataTableManger::Instance().GetDataTable<StringTable>(DataTable::Types::String))
#define ITEM_TABLE (DataTableManger::Instance().GetDataTable<ItemTable>(DataTable::Types::Item))
#define BLOCK_TABLE (DataTableManger::Instance().GetDataTable<Block1Table>(DataTable::Types::Block1))
#define MONSTER_TABLE (DataTableManger::Instance().GetDataTable<MonsterTable>(DataTable::Types::Monster))
#define STAGE1_TABLE (DataTableManger::Instance().GetDataTable<Stage1MapTable>(DataTable::Types::Stage1Map))
#define PARTICLE_TABLE (DataTableManger::Instance().GetDataTable<ParticleTable>(DataTable::Types::Particle))