#include "stdafx.h"
#include "LBP.h"
#include <iostream>


void CVC::OriginalLBP(const cv::Mat& srcImage, cv::Mat& desImage)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		OriginalLBP8Bit(imageRGB[c], imageLBP[c]);
	cv::merge(imageLBP, srcImage.channels(), desImage);
	delete[] imageLBP;
	delete[] imageRGB;
}

void CVC::OriginalLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage)
{
	// 1. padding
	cv::Mat padImage;
	cv::copyMakeBorder(srcImage, padImage, 1, 1, 1, 1, cv::BORDER_CONSTANT, 0);
	desImage = cv::Mat(srcImage.rows, srcImage.cols, CV_8UC1);

	// 2. calculate original lbp
	for (int row = 1; row < padImage.rows - 1; row++)
	{
		for (int col = 1; col <  padImage.cols - 1; col++)
		{
			UCHAR sum = 0;
			UCHAR centerPixel = padImage.at<UCHAR>(row, col);

			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row - 1, col - 1) > centerPixel) << 7;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row - 1, col) > centerPixel) << 6;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row - 1, col + 1) > centerPixel) << 5;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row, col + 1) > centerPixel) << 4;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row + 1, col + 1) > centerPixel) << 3;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row + 1, col) > centerPixel) << 2;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row + 1, col - 1) > centerPixel) << 1;
			sum |= static_cast<UCHAR>(padImage.at<UCHAR>(row, col - 1) > centerPixel);

			desImage.at<UCHAR>(row - 1, col - 1) = sum;
		}

	}
}

void CVC::CircularLBP(const cv::Mat& srcImage, cv::Mat& desImage,
	C_UINT32 radius, C_UINT32 bin)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		CircularLBP8Bit(imageRGB[c], imageLBP[c], radius, bin);
	cv::merge(imageLBP, srcImage.channels(), desImage);
	
	delete[] imageLBP;
	delete[] imageRGB;
}

void CVC::CircularLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage,
	C_UINT32 radius, C_UINT32 bin)
{
	// 1. padding
	cv::Mat padImage;
	cv::copyMakeBorder(srcImage, padImage, radius, radius, radius, radius, cv::BORDER_CONSTANT, 0);
	desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);

	UINT32 offsetBase = bin - 1;

	for (UINT32 index = 0; index < bin; index++)
	{
		C_FLOAT centerX = CVC::FixValue(static_cast<float>(radius * cos(2.0 * CVC::PI * index / bin)));
		C_FLOAT centerY = CVC::FixValue(static_cast<float>(radius * sin(2.0 * CVC::PI * index / bin)));

		C_INT32 x1 = static_cast<int32_t>(floor(centerX));
		C_INT32 y1 = static_cast<int32_t>(floor(centerX));
		C_INT32 x2 = static_cast<int32_t>(ceil(centerX));
		C_INT32 y2 = static_cast<int32_t>(ceil(centerX));

		C_FLOAT xOffset = centerX - x1;
		C_FLOAT yOffset = centerY - y1;

		//(y, x) -> (0, 0), (0, 1), (1, 0), (1, 1)
		C_FLOAT w1 = (1.0f - xOffset) * (1.0f - yOffset);
		C_FLOAT w2 = xOffset * (1.0f - yOffset);
		C_FLOAT w3 = (1.0f - xOffset) * yOffset;
		C_FLOAT w4 = xOffset * yOffset;

		// 2. calcuulate pixel
		for (UINT32 row = radius; row < padImage.rows - radius; row++)
		{
			for (UINT32 col = radius; col < padImage.cols - radius; col++)
			{
				UCHAR pix = 0;

				pix = static_cast<UCHAR>(padImage.at<UCHAR>(row + x1, col + y1) * w1
					+ padImage.at<UCHAR>(row + x2, col + y1) * w2
					+ padImage.at<UCHAR>(row + x1, col + y2) * w3
					+ padImage.at<UCHAR>(row + x2, col + y2) * w4);
				
				desImage.at<UCHAR>(row - radius, col - radius) |= (static_cast<UCHAR>(pix > padImage.at<UCHAR>(row, col)) << offsetBase);
			}
		}
		offsetBase--;
	}
}

void  CVC::InvariantLBP(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 radius, C_UINT32 bin)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		InvariantLBP8Bit(imageRGB[c], imageLBP[c], radius, bin);
	cv::merge(imageLBP, srcImage.channels(), desImage);

	delete[] imageLBP;
	delete[] imageRGB;
}

void  CVC::InvariantLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 radius, C_UINT32 bin)
{
	// 1. get circular lbp
	cv::Mat circularLBP;
	CircularLBP8Bit(srcImage, circularLBP, radius, bin);

	// 2. calculate invariant
	desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);

	for (int row = 0; row < circularLBP.rows; row++)
	{
		for (int col = 0; col < circularLBP.cols; col++)
		{
			// 使用位移(二進位)方式循環一次
			UCHAR originalPix = circularLBP.at<UCHAR>(row, col);
			UCHAR minPix = originalPix;

			for (UINT32 index = 1; index < bin; index++)
			{
				C_UCHAR temp = (originalPix >> index) | (originalPix << index);
				if (minPix > temp)
				{
					minPix = temp;
				}
			}

			desImage.at<UCHAR>(row, col) = minPix;
		}
	}
}

void CVC::EquivalentLBP(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 radius, C_UINT32 bin)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		EquivalentLBP8Bit(imageRGB[c], imageLBP[c], radius, bin);
	cv::merge(imageLBP, srcImage.channels(), desImage);

	delete[] imageLBP;
	delete[] imageRGB;
}

void CVC::EquivalentLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 radius, C_UINT32 bin)
{
	// 1. get circular lbp
	cv::Mat circularLBP;
	CircularLBP8Bit(srcImage, circularLBP, radius, bin);


	// 2. set everyone equivalent of the pixels
	UCHAR table[256] = { 0 };
	EquivalentTable(table);

	// 3. change to equivalent lbp
	desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);

	for (int row = 0; row < srcImage.rows; row++)
	{
		for (int col = 0; col < srcImage.cols; col++)
		{
			desImage.at<UCHAR>(row, col) = table[circularLBP.at<UCHAR>(row, col)];
		}
	}
}

void CVC::EquivalentTable(UCHAR* table)
{
	UCHAR pix = 1;

	for (UINT32 index = 0; index < 256; index++)
	{
		UINT32 count = 0;
		std::bitset<8> binary = index;

		for (UINT32 bitIndex = 0; bitIndex < 8; bitIndex++)
		{
			if (binary[bitIndex] != binary[(bitIndex + 1) % 8])
			{
				count++;
			}
		}

		if (count < 3)
		{
			table[index] = pix;
			pix++;
		}
	}
}

void  CVC::MultiScaleBlockLBP(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 scale)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		MultiScaleBlockLBP8Bit(imageRGB[c], imageLBP[c], scale);
	cv::merge(imageLBP, srcImage.channels(), desImage);

	delete[] imageLBP;
	delete[] imageRGB;
}

void  CVC::MultiScaleBlockLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 scale)
{
	C_UINT32 cellSize = scale;
	C_INT32 cellRadius = cellSize >> 1;

	// 1. padding
	cv::Mat avgImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
	cv::Mat padImage;
	cv::copyMakeBorder(srcImage, padImage, cellRadius, cellRadius, cellRadius, cellRadius, cv::BORDER_CONSTANT, 0);
	desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);

	for (int row = cellRadius; row < padImage.rows - cellRadius; row++)
	{
		for (int col = cellRadius; col < padImage.cols - cellRadius; col++)
		{
			int sum = 0;

			for (int cellRow = -cellRadius; cellRow < cellRadius; cellRow++)
			{
				for (int cellCol = -cellRadius; cellCol < cellRadius; cellCol++)
				{
					sum += padImage.at<UCHAR>(row + cellRow, col + cellCol);
				}
			}

			avgImage.at<UCHAR>(row - cellRadius, col - cellRadius) = static_cast<UCHAR>(sum / (cellSize * cellSize));
		}
	}
	// 3. calculate original lbp
	OriginalLBP(avgImage, desImage);
}

void  CVC::SEMultiScaleBlockLBP(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 scale)
{
	cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
	cv::split(srcImage, imageRGB);

	cv::Mat *imageLBP = new cv::Mat[srcImage.channels()];
	for (int c = 0; c < srcImage.channels(); c++)
		SEMultiScaleBlockLBP8Bit(imageRGB[c], imageLBP[c], scale);
	cv::merge(imageLBP, srcImage.channels(), desImage);

	delete[] imageLBP;
	delete[] imageRGB;
}

void  CVC::SEMultiScaleBlockLBP8Bit(const cv::Mat& srcImage, cv::Mat& desImage,
	C_INT32 scale)
{
	// 1. get block lbp
	cv::Mat MBLBP;
	MultiScaleBlockLBP8Bit(srcImage, MBLBP, scale);


	// 2. get histogram of the block lbp
	int32_t histogram[256] = { 0 };
	CVC::SetHistogram8bit(MBLBP, histogram);


	// 3. copy and sort the histogram
	int32_t sortHistogram[256] = { 0 };
	memcpy(sortHistogram, histogram, 256 * sizeof(int32_t));
	std::sort(sortHistogram, sortHistogram + 256);


	// 4. found top 64 of the sort histogram
	UCHAR table[256] = { 0 };

	for (UINT32 index = 0; index < 63; index++)
	{
		for (UINT32 hisIndex = 0; hisIndex < 256; hisIndex++)
		{
			if (sortHistogram[255 - index] == histogram[hisIndex])
			{
				table[hisIndex] = index;
			}
		}
	}


	// 5. set new pixel
	desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
	for (int row = 0; row < desImage.rows; row++)
	{
		for (int col = 0; col < desImage.cols; col++)
		{
			desImage.at<UCHAR>(row, col) = table[MBLBP.at<UCHAR>(row, col)];
		}
	}
}
