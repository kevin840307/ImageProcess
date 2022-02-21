#pragma once
#ifndef LBP_H
#define LBP_H
#include "general.h"
#include <bitset>

namespace CVC {

	void OriginalLBP(const cv::Mat& srcImage, cv::Mat& desImag);

	void OriginalLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImag);

	void CircularLBP(const cv::Mat& srcImage, cv::Mat& desImage
		, C_UINT32 radius = 1, C_UINT32 bin = 8);

	void CircularLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage
		, C_UINT32 radius = 1, C_UINT32 bin = 8);

	void InvariantLBP(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 radius = 1, C_UINT32 bin = 8);

	void InvariantLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 radius = 1, C_UINT32 bin = 8);

	void EquivalentLBP(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 radius = 1, C_UINT32 bin = 8);

	void EquivalentLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 radius = 1, C_UINT32 bin = 8);

	void MultiScaleBlockLBP(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 scale = 9);

	void MultiScaleBlockLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 scale = 9);

	void SEMultiScaleBlockLBP(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 scale = 9);

	void SEMultiScaleBlockLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage
		, C_INT32 scale = 9);

	void LBPHistogram(C_UCHAR* src
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 gridX, C_UINT32 gridY
		, C_UINT32 bin = 256);

	void LBPHistogram(C_UCHAR* src, std::vector<double>& histogram
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 gridX, C_UINT32 gridY
		, C_UINT32 bin = 256);

	void EquivalentTable(UCHAR* table);


}

#endif