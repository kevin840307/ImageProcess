#pragma once
#ifndef HOG_H
#define HOG_H
#include "general.h"

namespace CVC
{
	void HOGVisualization(const cv::Mat& srcImage, cv::Mat& desImag,
		C_INT32 cell = 8, C_INT32 block = 16);
}


#endif