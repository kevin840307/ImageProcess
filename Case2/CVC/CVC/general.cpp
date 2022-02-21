#include "stdafx.h"
#include "general.h"

//extern void MNDT::Write(C_UCHAR* str)
//{
//	std::fstream fwLog;
//
//	fwLog.open(MNDT::LOG_FILE, std::ios::app);
//	fwLog << str;
//	fwLog.close();
//}

namespace CVC
{
	void SetHistogram8bit(const cv::Mat& srcImage, int32_t* histogram)
	{
		for (int row = 0; row < srcImage.rows; row++)
		{
			for (int col = 0; col < srcImage.cols; col++)
			{
				histogram[srcImage.at<UCHAR>(row, col)]++;
			}
		}
	}

	char* LOG_FILE = "D:\\Log.txt";
	double PI = 3.1415926535;
}