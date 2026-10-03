#pragma once

namespace Objects
{
	extern size_t GetObjCount();
	extern dl_object_t* GetObjList();
	extern dl_object_t* GetObjById(size_t objId);

	extern bool bLog;
}