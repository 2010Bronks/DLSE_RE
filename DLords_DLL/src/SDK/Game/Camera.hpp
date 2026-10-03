#pragma once

struct CViewVec_VTable
{
	void (*Process)(struct CViewVec* that, int* a2);
};

struct CViewVec
{
	CViewVec_VTable* vtbl;

	char szVar[64];
	const char* dword44;
	const char* dword48;
	const char* dword4C;
	const char* dword50;
	float float54;
	float float58;
	float float5C;
	float float60;
	float float64;
	float float68;
	float float6C;
	float float70;
	float float74;
	float float78;
	float float7C;
	float float80;
	float float84;
	float float88;
	float float8C;
	void* dword90;
	int dword98;
	int _dword98;
	void* dword9C;
	int dwordA0;
	void* dwordA4;
	char byteA8;
	void* dwordAC;
	char byteB0;
	char byteB1;
	float floatB4;
};
static_assert(sizeof(CViewVec) == 184);

struct CCameraMovement_VTable
{
	void (*Destructor)(struct CCameraMovement* that, bool dispose);
	void (*Process)(struct CCameraMovement* that, float a2, int a3);
};

struct CCameraMovement
{
	CCameraMovement_VTable* vtbl;
	char gap4[4];
	void* pGeMeshModel;
	void* dwordC;
	float float10;
	float float14;
	float float18;
	float float1C;
	float float20;
	float float24;
	float float28;
	float float2C;
	float float30;
	float float34;
	float float38;
	float float3C;
	float float40;
	float float44;
	float float48;
	float float4C;
	float float50;
	void* pGxGNode;
	void* dword58;
	void* pGxGNode2;
	void* dword60;
	char byte64;
	char byte65;
	float float68;
	float float6C;
	float float70;
	CViewVec* pViewVec;
	char char78;
	char gap79[47];
	float floatA8;
	float floatAC;
	float floatB0;
	char gapB4[120];
	void* dword12C;
	void* dword130;
};
static_assert(sizeof(CCameraMovement) == 308);