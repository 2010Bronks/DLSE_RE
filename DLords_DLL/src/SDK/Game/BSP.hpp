#pragma once
#pragma pack(push, 1)

namespace DLords::BSP
{
	constexpr auto MAX_MAP_FACES = 80000;
	constexpr auto MAX_MAP_NODES = 40000;
	constexpr auto MAX_NAV_LOOKUP = 20000;
	constexpr auto MAX_FOUNTAINS = 16;
}

// 4 bytes — texture info (lump 11)
struct bsp_texinfo_t
{
	int textureIndex;   // +0x00 — index into pTextures
};

// 8 bytes — texture reference (lump 12, points to TWAD archive)
struct bsp_texture_t
{
	int twadOffset;   // +0x00 — offset in TWAD archive
	int twadIndex;    // +0x04 — TWAD file index
};

// 12 bytes — BSP node (lump 9)
struct bsp_node_t
{
	int planeIndex;   // +0x00 — index into pPlanes
	int frontChild;   // +0x04 — front child index (negative = leaf)
	int backChild;    // +0x08 — back child index (negative = leaf)
};

// 12 bytes — vertex (lump 3, NOT faces!)
// Loaded via BspLoadLump(3, ..., 12)
struct bsp_vertex_t
{
	float x;   // +0x00
	float y;   // +0x04
	float z;   // +0x08
};

// 16 bytes — cluster bbox boundary (NO padding, unlike bsp_plane_t!)
struct bsp_bound_t
{
	vector3 normal;   // +0x00
	float   offset;   // +0x0C
};

// 20 bytes — splitting plane (lump 1)
struct bsp_plane_t
{
	vector3 normal;   // +0x00
	float   offset;   // +0x0C — plane distance from origin
	float   padding;  // +0x10 — unused
};

// 26 bytes — lightmap descriptor (lump 7)
struct bsp_lightmap_t
{
	int8_t data0[4];              // +0x00
	int   type;    // +0x04 (1 = skip)
	int8_t data2[6];              // +0x08..+0x0E
	int16_t markSurfaceIndex;      // +0x0E — index into pMarkSurfaces
	int8_t data3[10];             // +0x10..+0x1A
};

// 32 bytes — leaf (lump 5, extended node structure)
struct bsp_leaf_t
{
	int  planeIndex;   // +0x00
	int  frontChild;   // +0x04
	int  backChild;    // +0x08
	int8_t gap0C[20];   // +0x0C..+0x20 — additional fields (bbox, content, pvsOffset?)
	int  unk20;        // +0x20
	int  unk24;        // +0x24
	int  unk28;        // +0x28
	int  unk2C;        // +0x2C
};

// 32 bytes — per-leaf content flags (lump 10)
// contentFlags values: -1=empty, -2=solid, -3=special, -5=visible+lit
struct bsp_leaf_info_t
{
	int  contentFlags;   // +0x00
	int8_t padding[28];   // +0x04
};

// 44 bytes — precomputed renderface (built at load time)
// Created in BspProcessRenderFaces (sub_5320A0)
// Stores precomputed visibility/lightmap flags for each face
struct bsp_renderface_pre_t
{
	int8_t data[28];          // +0x00..+0x1C — vertex indices, texture, lightmap refs
	int   flags;             // +0x1C — render flags:
	//   bit 0x01 = FLAG_VISIBLE
	//   bit 0x02 = FLAG_HAS_LIGHTMAP
	//   bit 0x04 = FLAG_VISIBLE_ALT
	//   bit 0x08 = FLAG_HIDDEN
	//   bit 0x10 = FLAG_NEEDS_LIGHTING
	//   bit 0x20 = FLAG_LIGHTMAP_DIRTY
	int8_t data2[12];         // +0x20..+0x2C — additional data
};

// 44 bytes — runtime light entity (from .lgt/.rgb/.int files)
struct light_entity_t
{
	float  intensity;     // +0x00
	float  unk04;         // +0x04
	vector3 pos;          // +0x08..+0x14 — world position
	int8_t  colorR;        // +0x14 — red channel
	int8_t  colorG;        // +0x18 — green channel
	int8_t  colorB;        // +0x1C — blue channel
	float  unk20;         // +0x20
	float  unk24;         // +0x24
	int    isInList;      // +0x28 — flag "added to spatial hash"
	int    unk2C;         // +0x2C
	int    unk30;         // +0x30
	int    unk34;         // +0x34
	int    unk38;         // +0x38
	int    unk3C;         // +0x3C
};

// 52 bytes — marksurface (lump 6)
struct bsp_marksurface_t
{
	int8_t data0[4];     // +0x00
	int   type;         // +0x04 (1 = skip)
	int8_t data2[36];    // +0x08..+0x2C
	int   textureId;    // +0x2C (bit 15 = 0x8000 = FLAG_VISIBLE)
	int8_t data3[16];    // +0x30..+0x34
};

// 52 bytes — runtime texture descriptor
// Created in BspLoadTwadTextures when loading .pal/.tex files
struct tex_desc_t
{
	int    textureId;        // +0x00 — texture ID (filled later)
	void* pixelOffset;      // +0x04 — offset into pPalette (runtime)
	int    unk08;            // +0x08 — pixelData (used in BspSpecialRender)
	int    unk0C;            // +0x0C
	int    unk10;            // +0x10
	int    unk14;            // +0x14
	int    unk18;            // +0x18
	int    unk1C;            // +0x1C
	int    unk20;            // +0x20
	int    unk24;            // +0x24
	short  textureSize;      // +0x28 — texture size (used in BspSpecialRender)
	short  unk2A;            // +0x2A
	int    unk2C;            // +0x2C
	int    unk30;            // +0x30
};

// 68 bytes — cluster/region (lump 14)
struct bsp_cluster_t
{
	int          selfSize;          // +0x00 = 68
	bsp_bound_t  bound1;            // +0x04 (16 bytes) — bbox mins
	bsp_bound_t  bound2;            // +0x14 (16 bytes) — bbox maxs
	int          bspTreeRoots[5];   // +0x24 (20 bytes) — 5 hulls:
	//   [0] = point hull   (+0x24)
	//   [1] = player hull  (+0x28)
	//   [2] = monster hull (+0x2C)
	//   [3] = large hull   (+0x30)
	//   [4] = vehicle hull (+0x34)
	int          firstFaceIndex;    // +0x38
	int          faceCount;         // +0x3C
	int          modelEntityCount;  // +0x40
};

// 92 bytes — entity / moving model (dual purpose!)
// Parsed from entity lump, also used for moving models (doors, elevators)
struct bsp_entity_t
{
	int    d6type;            // +0x00 (modelType: 0=slider, 2=rotating)
	int    d6axis;            // +0x04 (axisIndex: 0=X, 1=Y, 2=Z)
	float  d6maxmove;         // +0x08 (maxMove)
	int    d6entid;           // +0x0C
	int    modelIndex;        // +0x10 (clusterIndex)
	int    d6flags;           // +0x14
	float  d6rotx;            // +0x18 (origin.x for moving model)
	float  d6roty;            // +0x1C (origin.y)
	float  d6rotz;            // +0x20 (origin.z)
	float  currentValue;      // +0x24 (current angle/position)
	float  rotMatrix[9];      // +0x28..+0x48 (m3x3_t, identity by default)
	char   d6string[16];      // +0x4C..+0x5B
	char   pad5C;             // +0x5C
	// sizeof = 92 (0x5C)
};

// ============================================================================
// MAIN STRUCTURE — sizeof = 0x130934 (1247540 bytes)
// ============================================================================
struct bsp_data_t
{
	// === Lump pointers and counts (filled in LoadBspFile) ===
	int clusterCount;					// +0x00 (lump 14 count)
	bsp_cluster_t* pClusters;			// +0x04 (lump 14, 68 bytes)
	int clipnodeCount;					// +0x08 (lump 4 count)
	void* pClipnodes;					// +0x0C (lump 4)
	int lightmapArraySize;				// +0x10 (lump 8 size)
	void* pLightmapArray;				// +0x14 (lump 8, RGB565 pixels)
	int rawCount;						// +0x18 (lump 0 size)
	char* pRawData;						// +0x1C (lump 0, entity lump text)
	int leafInfoCount;					// +0x20 (lump 10 count)
	bsp_leaf_info_t* pLeafInfo;			// +0x24 (lump 10, 32 bytes)
	int planeCount;						// +0x28 (lump 1 count)
	bsp_plane_t* pPlanes;				// +0x2C (lump 1, 20 bytes)
	int faceCount;						// +0x30 (lump 3 count)
	bsp_vertex_t* pFaces;				// +0x34 (lump 3, 12 bytes — VERTICES, not faces!)
	int leafCount;						// +0x38 (lump 5 count)
	bsp_leaf_t* pLeaves;				// +0x3C (lump 5, 32 bytes)
	int markSurfCount;					// +0x40 (lump 6 count)
	bsp_marksurface_t* pMarkSurfaces;	// +0x44 (lump 6, 52 bytes)
	int lightmapCount;					// +0x48 (lump 7 count)
	bsp_lightmap_t* pLightmaps;			// +0x4C (lump 7, 26 bytes)
	void* pLightmapBuffer;				// +0x50 (runtime, 4 * lightmapCount)
	int texIndexedFlag;					// +0x54 (1 = textures loaded)
	void* pTextureIndices;				// +0x58
	tex_desc_t* pTextureDescriptors;	// +0x5C (52 bytes, runtime)
	int palSize;						// +0x60
	void* pPalette;						// +0x64
	int nodeCount;						// +0x68 (lump 9 count)
	bsp_node_t* pNodes;					// +0x6C (lump 9, 12 bytes)
	int textureCount;					// +0x70 (lump 12 count)
	bsp_texture_t* pTextures;			// +0x74 (lump 12, 8 bytes)
	int texInfoCount;					// +0x78 (lump 11 count)
	bsp_texinfo_t* pTexInfo;			// +0x7C (lump 11, 4 bytes)
	int pvsDataCount;					// +0x80 (lump 13 count)
	void* pPvsData;						// +0x84 (lump 13, 4 bytes)

	// === Internal data (gap70 region) ===
	int8_t gap88[4];					// +0x88
	int entityCount;					// +0x8C
	// +0x90: entities[] / movingModels[] — 92 bytes each
	//        (variable-length array, max ~1000 entries)
	//        Also contains:
	//          entityById[]        at offset +0xB920 (int[], indexed by d6entid)
	//          entityToModelMap[]  at offset +0x5CB0 (int16[], indexed by entNum)
	int8_t gap90[0x5C90 - 0x90];		// +0x90..+0x5C90 (23552 bytes)
	int8_t pvsFallback[0x2000];			// +0x5C90..+0x7C90 (8192 bytes, init 0xFF)
	int renderFaceCount;				// +0x7C90
	bsp_renderface_pre_t* pRenderFaces;	// +0x7C94 (44 bytes each)
	int8_t gap7C98[0x1B4FC - 0x7C98];	// +0x7C98..+0x1B4FC (79972 bytes)

	// === Regional bbox and origin (filled in TerrainBSPLoad) ===
	float worldMinsX;					// +0x1B4FC
	float worldMinsZ;					// +0x1B500
	float worldMaxsX;					// +0x1B504
	float worldMaxsZ;					// +0x1B508
	float originX;						// +0x1B50C
	float originY;						// +0x1B510
	float originZ;						// +0x1B514
	int8_t gap1B518[0x2D6E8 - 0x1B518];	// +0x1B518..+0x2D6E8 (74192 bytes)

	// === Lighting data and PVS ===
	int lightSourceCount;				// +0x2D6E8
	void* pPvsA;						// +0x2D6EC (PVS array A, 4 bytes per leaf)
	void* pPvsB;						// +0x2D6F0 (PVS array B, 4 bytes per leaf)
	int8_t gap2D6F4[4];					// +0x2D6F4
	int dword2D6F8;						// +0x2D6F8 (reset to 0)
	// +0x2D704: lightSources (inside gap2D6FC)
	// +0x2D714: lightData (inside gap2D6FC)
	int8_t gap2D6FC[3584];				// +0x2D6FC..+0x2E4FC
	int dword2E4FC;						// +0x2E4FC (reset to 0)
	// +0x2E53C: lightRenderCount (inside gap2E500)
	int8_t gap2E500[0x130500 - 0x2E500];// +0x2E500..+0x130500 (1056768 bytes)

	// === Filename and load flags ===
	char szBspFile[1024];				// +0x130500..+0x130900
	int bspIndex;						// +0x130900 (index in g_BspData[])
	int isLoaded;						// +0x130904 (= 1 after load)
	int mainLumpsLoaded;				// +0x130908 (= 1 after BspLoadMainLumps)
	int bspLoadFlags;					// +0x13090C (0 = main, !=0 = additional)

	// === Region lightmap UV coordinates ===
	float lightmapUOrigin;				// +0x130910
	float lightmapVOrigin;				// +0x130914 (= 0.0)
	float lightmapUScale;				// +0x130918
	float lightmapVScale;				// +0x13091C

	// === Memory tracking and light entities ===
	int totalAllocSize;					// +0x130920 (sum of all malloc)
	int mainLumpsAllocSize;				// +0x130924 (sum of main lumps)
	int lightEntityCount;				// +0x130928 (from .lgt file)
	light_entity_t** pLightEntities;	// +0x13092C (44 bytes each)
	int isMainBsp;						// +0x130930 (= 1 if bspLoadFlags == 0)
};
static_assert(sizeof(bsp_data_t) == 0x130934, "bsp_data_t size mismatch");

struct terrain_tile_t				// ~36 bytes
{
	float* pBaseHeight;				// +0x00 — pointer to base height value
	int    frameCounter;			// +0x08 — g_TerObjs[3] (last render frame)
	int    visibility;				// +0x0C — -1=not visible, 1=visible, 2=water
	int    textureX;				// +0x10
	int    textureZ;				// +0x12
	int    flags;					// +0x1C — bit 0x4000 = ?
	int    fogFactor;				// +0x1E (BYTE) — fog factor 0-255
	int    lodLevel;				// +0x1E (BYTE at +30) — LOD/fog 0-255
};
#pragma pack(pop)