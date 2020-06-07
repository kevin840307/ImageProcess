#pragma once
#ifndef LIBRARY_H
#define LIBRARY_H

#include <fstream>
#include <string>
#include <set>
#include "general.h"
#include "Image.h"
#include "draw.h"
#include "GaussianElimination.h"
#include "PerspectiveTransform.h"

enum ColerType
{
	BGR2GRAY_8BIT,
	BGR2HSV,
	HSV2BGR,
	BGR2YCbCr,
	YCbCr2BGR,
};


namespace IVC {
	/*
		ChangeColor Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		type		= change type
	*/
	void ChangeColor(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 type);

	/*
		ImagePadding8bit Parameter:
		src		= source of image
		pur		= purpose of image
		width		= Image width
		height		= Image height
		pad		= padding size
	*/
	void ImagePadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad);

	/*
		Resize Parameter:
		src			= source of image
		pur			= purpose of image
		width		= Image width
		height		= Image height
		reWidth		= new width
		reHeight	= new height
		type		= resize type
	*/
	void Resize8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 reWidth, C_UINT32 reHeight
		, IVC::ResizeType type);

	void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_FLOAT* points
		, IVC::PerspectiveTransType type);

	void PerspectiveTransformReverse8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_FLOAT* points
		, IVC::PerspectiveTransType type);

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height);
}

namespace IVC {
	namespace Inner {
		// change color start
		void BGR2Gray8Bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height);

		void NearestResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight);

		void LinearResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight);

		void BiCubicResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight);

		float BiCubicPloy(float x);

		void PerspectiveTransformNear8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);

		void PerspectiveTransformLinear8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);


		void PerspectiveTransformBiCubic8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);

		void PerspectiveTransformNearReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);

		void PerspectiveTransformLinearReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);


		void PerspectiveTransformBiCubicReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits);
	}
}

#endif // !LIBRARY_H