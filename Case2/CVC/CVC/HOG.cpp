#include "stdafx.h"
#include "HOG.h"
#include <iostream>

void CVC::HOGVisualization(const cv::Mat& srcImage, cv::Mat& desImag
	, C_INT32 cell, C_INT32 block)
{
	C_INT32 bins = 9;
	cv::Size winSize(srcImage.cols / cell * cell, srcImage.rows / cell * cell); // fixed from cellSize
	cv::Size blockSize(block, block);
	cv::Size blockStride(cell, cell);
	cv::Size cellSize(cell, cell);
	cv::HOGDescriptor hog(winSize, blockSize, blockStride, cellSize, bins);
	std::vector<float> descriptors;
	std::vector<cv::Point> locations;

	hog.compute(srcImage, descriptors, cv::Size(0, 0), cv::Size(0, 0), locations);
	

	C_FLOAT radRangeForOneBin = CVC::PI / (float)bins;
	C_INT32 cellXSize = winSize.width / cell;
	C_INT32 cellYSize = winSize.height / cell;
	C_INT32 blockXSize = cellXSize - 1;
	C_INT32 blockYSize = cellYSize - 1;
	C_INT32 cellOfBlockXSize = block / cell;
	C_INT32 cellOfBlockYSize = block / cell;
	
	std::vector<std::vector<std::vector<float>>> cellHistogram(cellYSize,
		std::vector<std::vector<float>>(cellXSize, std::vector<float>(bins, 0.0f)));
	std::vector<std::vector<float>> cellHisCount(cellYSize,
		std::vector<float>(cellXSize, 0.0f));


	int descriptorIndex = 0;
	for (int blockx = 0; blockx < blockXSize; blockx++)
	{
		C_INT32 regionXSize = blockx + cellOfBlockXSize;
		for (int blocky = 0; blocky < blockYSize; blocky++)
		{
			C_INT32 regionYSize = blocky + cellOfBlockYSize;
			for (int cellx = blockx; cellx < regionXSize; cellx++)
			{
				for (int celly = blocky; celly < regionYSize; celly++)
				{
					for (int bin = 0; bin < bins; bin++)
					{
						float gradientStrength = descriptors[descriptorIndex++];
						cellHistogram[celly][cellx][bin] += gradientStrength;
					}
					cellHisCount[celly][cellx]++;
				}
			}
		}
	}


	for (int celly = 0; celly < cellYSize; celly++)
	{
		for (int cellx = 0; cellx < cellXSize; cellx++)
		{
			C_FLOAT count = (float)cellHisCount[celly][cellx];

			for (int bin = 0; bin < bins; bin++)
			{
				cellHistogram[celly][cellx][bin] /= count;
			}
		}
	}


	desImag = srcImage.clone();
	// draw cells  
	for (int celly = 0; celly < cellYSize; celly++)
	{
		for (int cellx = 0; cellx < cellXSize; cellx++)
		{
			int drawX = cellx * cellSize.width;
			int drawY = celly * cellSize.height;

			int mx = drawX + cellSize.width / 2;
			int my = drawY + cellSize.height / 2;


			// draw in each cell all 9 gradient strengths  
			for (int bin = 0; bin < bins; bin++)
			{
				C_FLOAT currentGradStrength = cellHistogram[celly][cellx][bin];


				if (currentGradStrength == 0) continue;

				C_FLOAT currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

				C_FLOAT dirVecX = cos(currRad);
				C_FLOAT dirVecY = sin(currRad);
				C_FLOAT maxVecLen = cellSize.width / 2;
				C_FLOAT scale = 2.0f;

				C_FLOAT x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
				C_FLOAT y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
				C_FLOAT x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
				C_FLOAT y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;


				cv::line(desImag,
					cv::Point2f(x1, y1),
					cv::Point2f(x2, y2),
					CV_RGB(0, 0, 255),
					1);
			}
		} 
	}

}