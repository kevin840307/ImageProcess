#pragma once
#ifndef GENERAL_H
#define GENERAL_H
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <vector>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"


//#define NDEBUG
//#include <assert.h>

typedef unsigned char UCHAR;
typedef const unsigned char C_UCHAR;
typedef const char C_CHAR;
typedef unsigned __int32 UINT32;
typedef const unsigned __int32 C_UINT32;
typedef const __int32 C_INT32;
typedef const double C_DOUBLE;
typedef const float C_FLOAT;


namespace CVC {

	void SetHistogram8bit(const cv::Mat& srcImage, int32_t* histogram);
	
	template <typename T>
	inline void Write(T& data)
	{
		std::fstream fwLog;

		fwLog.open(MNDT::LOG_FILE, std::ios::app);
		fwLog << data;
		fwLog.close();
	}

	inline float FixValue(C_FLOAT& value)
	{
		return abs(value) < 0.00000001f ? 0 : value;
	}

	inline double FixValue(C_DOUBLE& value)
	{
		return abs(value) < 0.00000001 ? 0 : value;
	}

	extern char* LOG_FILE;
	extern double PI;
}
#endif // !GENERAL_H
