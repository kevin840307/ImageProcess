#ifdef IVCLIBRARY_EXPORTS
#define IVCLIBRARY_API __declspec(dllexport)
#include "Library.h"

extern "C" IVCLIBRARY_API void ivcChangeColor(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height
	, C_UINT32 type)
{

	IVC::ChangeColor(src, pur
		, width, height
		, type);
}

extern "C" IVCLIBRARY_API void ivcResize8bit(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height
	, C_UINT32 reWidth, C_UINT32 reHeight
	, C_UINT32 type)
{

	IVC::Resize8bit(src, pur
		, width, height
		, reWidth, reHeight
		, (IVC::ResizeType)type);
}

extern "C" IVCLIBRARY_API void ivcPerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height
	, C_FLOAT* linkPoints
	, C_UINT32 type)
{

	IVC::PerspectiveTransform8bit(src, pur
		, width, height
		, linkPoints
		, (IVC::PerspectiveTransType)type);
}

extern "C" IVCLIBRARY_API void ivcPerspectiveTransformReverse8bit(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height
	, C_FLOAT* linkPoints
	, C_UINT32 type)
{

	IVC::PerspectiveTransformReverse8bit(src, pur
		, width, height
		, linkPoints
		, (IVC::PerspectiveTransType)type);
}

extern "C" IVCLIBRARY_API void ivcContourFrame8bit(C_UCHAR* src, UCHAR* pur
	, C_UINT32 width, C_UINT32 height)
{

	IVC::ContourFrame8bit(src, pur
		, width, height);
}

#else
#define IVCLIBRARY_API __declspec(dllimport)
#endif