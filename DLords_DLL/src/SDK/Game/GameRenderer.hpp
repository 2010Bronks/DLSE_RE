#pragma once
#include <d3d9.h>

struct scr_info_t
{
	int iWidth;
	int iHeight;
	int iRefreshRate;
	int iFormat;
};

struct display_mode_t
{
	int id;
	int iWidth;
	int iHeight;
	int iRefreshRate;
};

struct texture_bits_t
{
	int b1;
	int b2;
	int b3;
	int b4;
};

struct texture_format_t
{
	int a;
	int b;
	texture_bits_t bits;
	int c;
	int d;
};

enum ETextureFormatType : uint32_t
{
	eAlpha,
	eMask,
	eRGB,
	eLMap,

	eTFT_Max
};

struct renderer_states_t
{
	int alpha_blend_enable;
	int alpha_test_enable;
	int alpha_ref;
	int alpha_func;
	int z_enable;
	int z_write_enable;
	int z_func;
	int dword_348A51C;
	int dword_348A520;
	int mag_filter;
	int min_filter;
	int cull_mode;
	int shade_mode;
	int src_blend;
	int dest_blend;
	int fog_enable;
	uint32_t fog_color;
	int fog_vertex_mode;
	float fog_start;
	float fog_end;
	int unk1;
	int range_fog_enable;
	int color_op;
	int color_arg1;
	int color_arg2;
	int fill_mode;
	int dither_enable;
	int specular_enable;
	int multi_sample_antialias;
	int alpha_op;
	int alpha_arg1;
	int alpha_arg2;
	int clipping;
	int lightning;
	int max_anisotropy;

	__forceinline bool operator==(const renderer_states_t& other)
	{
		return
			alpha_blend_enable == other.alpha_blend_enable &&
			alpha_test_enable == other.alpha_test_enable &&
			alpha_ref == other.alpha_ref &&
			alpha_func == other.alpha_func &&
			z_enable == other.z_enable &&
			z_write_enable == other.z_write_enable &&
			z_func == other.z_func &&
			dword_348A51C == other.dword_348A51C &&
			dword_348A520 == other.dword_348A520 &&
			mag_filter == other.mag_filter &&
			min_filter == other.min_filter &&
			cull_mode == other.cull_mode &&
			shade_mode == other.shade_mode &&
			src_blend == other.src_blend &&
			dest_blend == other.dest_blend &&
			fog_enable == other.fog_enable &&
			fog_color == other.fog_color &&
			fog_vertex_mode == other.fog_vertex_mode &&
			fog_start == other.fog_start &&
			fog_end == other.fog_end &&
			unk1 == other.unk1 &&
			range_fog_enable == other.range_fog_enable &&
			color_op == other.color_op &&
			color_arg1 == other.color_arg1 &&
			color_arg2 == other.color_arg2 &&
			fill_mode == other.fill_mode &&
			dither_enable == other.dither_enable &&
			specular_enable == other.specular_enable &&
			multi_sample_antialias == other.multi_sample_antialias &&
			alpha_op == other.alpha_op &&
			alpha_arg1 == other.alpha_arg1 &&
			alpha_arg2 == other.alpha_arg2 &&
			clipping == other.clipping &&
			lightning == other.lightning &&
			max_anisotropy == other.max_anisotropy;

	}

	__forceinline bool operator!=(const renderer_states_t& other)
	{
		return
			alpha_blend_enable != other.alpha_blend_enable ||
			alpha_test_enable != other.alpha_test_enable ||
			alpha_ref != other.alpha_ref ||
			alpha_func != other.alpha_func ||
			z_enable != other.z_enable ||
			z_write_enable != other.z_write_enable ||
			z_func != other.z_func ||
			dword_348A51C != other.dword_348A51C ||
			dword_348A520 != other.dword_348A520 ||
			mag_filter != other.mag_filter ||
			min_filter != other.min_filter ||
			cull_mode != other.cull_mode ||
			shade_mode != other.shade_mode ||
			src_blend != other.src_blend ||
			dest_blend != other.dest_blend ||
			fog_enable != other.fog_enable ||
			fog_color != other.fog_color ||
			fog_vertex_mode != other.fog_vertex_mode ||
			fog_start != other.fog_start ||
			fog_end != other.fog_end ||
			unk1 != other.unk1 ||
			range_fog_enable != other.range_fog_enable ||
			color_op != other.color_op ||
			color_arg1 != other.color_arg1 ||
			color_arg2 != other.color_arg2 ||
			fill_mode != other.fill_mode ||
			dither_enable != other.dither_enable ||
			specular_enable != other.specular_enable ||
			multi_sample_antialias != other.multi_sample_antialias ||
			alpha_op != other.alpha_op ||
			alpha_arg1 != other.alpha_arg1 ||
			alpha_arg2 != other.alpha_arg2 ||
			clipping != other.clipping ||
			lightning != other.lightning ||
			max_anisotropy != other.max_anisotropy;
	}
};
static_assert(sizeof(renderer_states_t) == 140);

struct d3d_data_t
{
	IDirect3D9* pData;
	IDirect3DDevice9* pDevice;
	D3DCAPS9 Caps;
	texture_format_t aTextureFormats[ETextureFormatType::eTFT_Max];

	uint32_t iAvailableTextureMem;
	uint32_t iAvailableTextureMem2;
	uint8_t data3[77100];

	bool IsReady() const;
};
static_assert(sizeof(d3d_data_t) == 77548);

struct renderer_data_t
{
	int iAdapterCount;				// 0
	int iAdapter;					// 4
	int iPixelSize;					// 8
	D3DFORMAT eFormat;				// 12
	D3DFORMAT eZBufferFormat;		// 16
	int iDisplayModeCount;			// 20
	scr_info_t* pDisplayModes;		// 24

	display_mode_t sCurDisplayMode;	// 40

	d3d_data_t* pD3d;				// 44

	uint8_t unk5[4];
	RECT rectScreen;
	POINT ptMousePos;
	//int iWidth;
	//int iHeight;
	IDirect3DSurface9* pBackBuffer;
	int unk1;

	bool IsReady() const;
	IDirect3DDevice9* GetDevice();
};

struct cursor_prop_t
{
	UINT XHotSpot;
	UINT YHotSpot;
	IDirect3DSurface9 *pCursorBitmap;
};

//static constexpr auto mda = offsetof(dl_d3d_data_t, something);
static_assert(offsetof(renderer_data_t, pD3d) == 44);
static_assert(sizeof(renderer_data_t) == 84);

struct RawFrame4444
{
	int width;
	int height;
	unsigned short* pixels;
};

// BMP file structure
struct RawFrame8888
{
	int width;
	int height;
	unsigned int* pixels;
};

struct ProcessedFrame
{
	IDirect3DTexture9* texture;
	UINT  newWidth;
	UINT  newHeight;
	float scaleX;
	float scaleY;
	int bytesPerPixel;
};

struct AnimTexAlpha4444
{
	int numFrames;
	RawFrame4444* rawFrames;
	ProcessedFrame* processedFrames;
};

struct AnimTexAlpha8888
{
	int numFrames;
	RawFrame8888* rawFrames;
	ProcessedFrame* processedFrames;
};

struct GraphicResource
{
	const char* fname;
	int type;
	int extra;
};

struct GameMap_t
{
	int currentSpoke;
	int numOfMaps;
	int idx[8];
	int isLoaded[8];
	RawFrame8888* rawBmp[8];
	ProcessedFrame* vtex[8];
	RawFrame8888* rawBmp16[8];
	ProcessedFrame* maskVtex[8];
};