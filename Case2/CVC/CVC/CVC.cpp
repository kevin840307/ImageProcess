// CVC.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/ximgproc/segmentation.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core.hpp>
#include "general.h"
#include <iostream>
#include <ctime>
#include <unordered_map>
#include "LBP.h"
#include "HOG.h"
#include "KalmanTracker.h"
#include "Hungarian.h"
#include <fstream>
#include <unordered_map>



enum Functions
{
	cvtColor,
	dilate,
	erode,
	blur,
	GaussianBlur,
	medianBlur,
	threshold,
	threshold_otsu,
	Canny,
	Sobel,
	Laplacian,
	equalizeHist,
	Laplace,
	LogTransformation,
	GammaCorrection,
	HistVisualization,
	flip,
	resize,
	rotate,
	translate,
	OriginalLBP,
	CircularLBP,
	InvariantLBP,
	EquivalentLBP,
	MultiScaleBlockLBP,
	SEMultiScaleBlockLBP,
	HOG,
	felzenszwalb,
	SelectiveSearch,
	HoughLinesP,
	HoughCircles
};


std::string _fuctions[] = {
	"cvtColor",
	"dilate",
	"erode" ,
	"blur" ,
	"GaussianBlur" ,
	"medianBlur",
	"threshold",
	"threshold_otsu",
	"Canny",
	"Sobel",
	"Laplacian",
	"equalizeHist",
	"Laplace",
	"LogTransformation",
	"GammaCorrection",
	"HistVisualization",
	"flip",
	"resize",
	"rotate",
	"translate",
	"OriginalLBP",
	"CircularLBP",
	"InvariantLBP",
	"EquivalentLBP",
	"MultiScaleBlockLBP",
	"SEMultiScaleBlockLBP",
	"HOG",
	"felzenszwalb",
	"SelectiveSearch",
	"HoughLinesP",
	"HoughCircles"
};

std::string _errors_message[] = {
	"Suceess",
	"Unknown Error",
	"Type Error"
};

namespace CVC
{
	//f(x) = c * log(x)
	void LogTransformation(const cv::Mat& srcImage, cv::Mat& desImage, double c = 1)
	{
		srcImage.convertTo(desImage, CV_32F);
		desImage = desImage + 1;
		cv::log(desImage, desImage);
		desImage *= c;
		cv::convertScaleAbs(desImage, desImage);
		cv::normalize(desImage, desImage, 0, 255, cv::NORM_MINMAX);
	}

	void GammaCorrection(const cv::Mat& srcImage, cv::Mat& desImage, double gamma = 1)
	{
		cv::Mat lookUpTable(1, 256, CV_8U);
		uchar* p = lookUpTable.ptr();
		for (int i = 0; i < 256; ++i)
			p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
		desImage = srcImage.clone();
		cv::LUT(srcImage, lookUpTable, desImage);
	}

	void EqualizeHist(const cv::Mat& srcImage, cv::Mat& desImage)
	{
		cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
		cv::split(srcImage, imageRGB);
		for (int c = 0; c < srcImage.channels(); c++)
			cv::equalizeHist(imageRGB[c], imageRGB[c]);
		cv::merge(imageRGB, srcImage.channels(), desImage);
		delete[] imageRGB;
	}

	void HistVisualization(const cv::Mat& srcImage, cv::Mat& desImage, C_UINT32 histW = 512, C_UINT32 histH = 400)
	{

		int histSize = 256;
		float range[] = { 0, 256 }; //the upper boundary is exclusive
		C_FLOAT* histRange = { range };
		bool uniform = true, accumulate = false;



		cv::Mat* hists = new cv::Mat[srcImage.channels()];
		cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
		cv::split(srcImage, imageRGB);
		for (int c = 0; c < srcImage.channels(); c++)
			calcHist(&imageRGB[c], 1, 0, cv::Mat(), hists[c], 1, &histSize, &histRange, uniform, accumulate);
		delete[] imageRGB;



		int bin_w = cvRound((double)histW / histSize);
		C_UINT32 type = srcImage.channels() == 3 ? CV_8UC3 : CV_8UC1;
		cv::Scalar initScalar = srcImage.channels() == 3 ? cv::Scalar(0, 0, 0) : cv::Scalar(0);
		desImage = cv::Mat(histH, histW, type, initScalar);
		cv::Scalar colors[] = { cv::Scalar(255, 0, 0) , cv::Scalar(0, 255, 0) , cv::Scalar(0, 0, 255) };
		for (int c = 0; c < srcImage.channels(); c++)
			cv::normalize(hists[c], hists[c], 0, desImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		for (int i = 1; i < histSize; i++)
			for (int c = 0; c < srcImage.channels(); c++)
				cv::line(desImage, cv::Point(bin_w*(i - 1), histH - cvRound(hists[c].at<float>(i - 1))),
					cv::Point(bin_w*(i), histH - cvRound(hists[c].at<float>(i))),
					colors[c], 2, 8, 0);
		delete[] hists;
	}

	void ThresholdOtsu(const cv::Mat& srcImage, cv::Mat& desImage)
	{
		cv::Mat *imageRGB = new cv::Mat[srcImage.channels()];
		cv::split(srcImage, imageRGB);
		for (int i = 0; i < srcImage.channels(); i++)
			cv::threshold(imageRGB[i], imageRGB[i], 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
		cv::merge(imageRGB, srcImage.channels(), desImage);
		delete[] imageRGB;
	}

	void Rotate(const cv::Mat& srcImage, cv::Mat& desImage, C_DOUBLE angle)
	{
		cv::Size srcSize = srcImage.size();
		cv::Size dstSize(srcSize.height, srcSize.width);
		int len = std::max(srcImage.cols, srcImage.rows);
		cv::Point2f center(len / 2.f, len / 2.f);
		cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);

		cv::warpAffine(srcImage, desImage, rotMat, dstSize);
	}

	void Translate(const cv::Mat& srcImage, cv::Mat& desImage, C_FLOAT x, C_FLOAT y)
	{
		cv::Size srcSize = srcImage.size();
		cv::Mat tMat = cv::Mat::zeros(2, 3, CV_32FC1);

		tMat.at<float>(0, 0) = 1;
		tMat.at<float>(0, 2) = x;
		tMat.at<float>(1, 1) = 1;
		tMat.at<float>(1, 2) = y;
		cv::warpAffine(srcImage, desImage, tMat, srcSize);

	}


	inline cv::Vec3b GetColor()
	{
		return cv::Vec3b(rand() % 255, rand() % 255, rand() % 255);
	}

	inline cv::Scalar Vec3b2Scalar(const cv::Vec3b& v)
	{
		return cv::Scalar(v[0], v[1], v[2]);
	}

	void felzenszwalb(const cv::Mat& srcImage, cv::Mat& desImage,
		C_DOUBLE sigma = 0.5, C_FLOAT k = 300, C_INT32 minSize = 100)
	{
		using namespace cv::ximgproc::segmentation;
		cv::Mat seg;
		cv::Ptr<GraphSegmentation> gs = createGraphSegmentation();
		gs->setSigma(sigma);
		gs->setK(k);
		gs->setMinSize(minSize);
		gs->processImage(srcImage, seg);



		double min, max;
		cv::minMaxLoc(seg, &min, &max);

		cv::Vec3b *colorMap = new cv::Vec3b[(int)max + 1];
		for (UINT32 index = 0; index < max + 1; index++)
			colorMap[index] = GetColor();

		desImage = cv::Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);

		for (int row = 0; row < seg.rows; row++)
		{
			for (int col = 0; col < seg.cols; col++)
			{
				cv::Vec3b &color = desImage.at<cv::Vec3b>(row, col);
				color = colorMap[seg.at<int>(row, col)];;
			}
		}
		delete[] colorMap;
	}

	void SelectiveSearch(const cv::Mat& srcImage, cv::Mat& desImage,
		C_FLOAT sigma = 0.8, C_INT32 baseK = 150, C_INT32 incK = 150, bool fast = true)
	{
		using namespace cv::ximgproc::segmentation;
		cv::Ptr<SelectiveSearchSegmentation> ss = createSelectiveSearchSegmentation();
		cv::Mat bgrImage;
		if (srcImage.channels() == 1)
		{
			cv::cvtColor(srcImage, bgrImage, cv::COLOR_GRAY2BGR);
			ss->setBaseImage(bgrImage);
		}
		else
		{
			bgrImage = srcImage;
		}

		ss->setBaseImage(bgrImage);

		if (fast)
			ss->switchToSelectiveSearchFast(baseK, incK, sigma);
		else
			ss->switchToSelectiveSearchQuality(baseK, incK, sigma);

		std::vector<cv::Rect> rects;
		ss->process(rects);

		desImage = srcImage.clone();
		for (int index = 0; index < rects.size() && index < 100; index++)
			cv::rectangle(desImage, rects[index], cv::Scalar(0, 255, 0));
	}




	void HoughLinesP(const cv::Mat& srcImage, cv::Mat& desImage,
		C_INT32 threshold = 10, C_DOUBLE minLineLength = 0)
	{

		cv::Mat canny;
		if (srcImage.channels() > 1)
			cv::Canny(srcImage, canny, 100, 200);
		else
			canny = srcImage;



		std::vector<cv::Vec4f> plines;
		cv::HoughLinesP(canny, plines, 1, CV_PI / 180, threshold, minLineLength, 0);

		cv::Scalar color = cv::Scalar(0, 0, 255);
		desImage = srcImage.clone();
		for (size_t index = 0; index < plines.size(); index++)
		{
			cv::Vec4f hline = plines[index];
			cv::line(desImage, cv::Point(hline[0], hline[1]), cv::Point(hline[2], hline[3]), color, 3, cv::LINE_AA);
		}
	}


	// HoughCircles	(	InputArray 	image,
	// OutputArray 	circles,
	// 	int 	method,
	// 	double 	dp,
	// 	double 	minDist,
	// 	double 	param1 = 100, Canny threshold(small)
	// 	double 	param2 = 100, Circles threshold(count of cos and sin)
	// 	int 	minRadius = 0,
	// 	int 	maxRadius = 0
	// 	)
	void HoughCircles(const cv::Mat& srcImage, cv::Mat& desImage,
		C_DOUBLE param2 = 30, C_INT32 minRadius = 1, C_INT32 maxRadius = 30)
	{
		cv::Mat canny;
		if (srcImage.channels() > 1)
			cv::Canny(srcImage, canny, 50, 150);
		else
			canny = srcImage;

		std::vector<cv::Vec3f> circles;
		cv::HoughCircles(canny, circles, cv::HOUGH_GRADIENT, 1,
			canny.rows / 16,  // change this value to detect circles with different distances to each other
			50, param2, minRadius, maxRadius);

		desImage = srcImage.clone();
		for (size_t index = 0; index < circles.size(); index++)
		{
			cv::Vec3i c = circles[index];
			cv::Point center = cv::Point(c[0], c[1]);
			// circle center
			cv::circle(desImage, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
			// circle outline
			int radius = c[2];
			cv::circle(desImage, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
		}
	}
}



int Commend(char *argv[])
{
	const Functions commend = static_cast<Functions>(atoi(argv[1]));
	const int readType = atoi(argv[2]);
	const std::string imgPath(argv[3]);
	const std::size_t index = imgPath.rfind('\\') + 1;
	const std::string path = imgPath.substr(0, index);
	const std::string filename = imgPath.substr(index, imgPath.length() - index);
	cv::Mat srcImage = cv::imread(imgPath, readType);
	cv::Mat desImage;

	switch (commend)
	{
	case Functions::cvtColor:
	{
		C_UINT32 colorType = atoi(argv[4]);
		cv::cvtColor(srcImage, desImage, colorType);
	}
	break;
	case Functions::dilate:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
		cv::dilate(srcImage, desImage, kernel);
	}
	break;
	case Functions::erode:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));
		cv::erode(srcImage, desImage, kernel);
	}
	break;
	case Functions::blur:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::blur(srcImage, desImage, cv::Size(kernelSize, kernelSize));
	}
	break;
	case Functions::GaussianBlur:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		C_DOUBLE sigmaX = atoi(argv[5]);
		C_DOUBLE sigmaY = atoi(argv[6]);
		cv::GaussianBlur(srcImage, desImage, cv::Size(kernelSize, kernelSize), sigmaX, sigmaY);
	}
	break;
	case Functions::medianBlur:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::medianBlur(srcImage, desImage, kernelSize);
	}
	break;

	case Functions::threshold:
	{
		C_UINT32 thresh = atoi(argv[4]);
		cv::threshold(srcImage, desImage, thresh, 255, CV_THRESH_BINARY);
	}
	break;
	case Functions::threshold_otsu:
	{
		CVC::ThresholdOtsu(srcImage, desImage);
	}
	break;
	case Functions::Canny:
	{
		C_UINT32 lowThreshold = atoi(argv[4]);
		C_UINT32 highThreshold = atoi(argv[5]);
		C_UINT32 kernelSize = atoi(argv[6]);
		cv::Canny(srcImage, desImage, lowThreshold, highThreshold, kernelSize);
	}
	break;
	case Functions::Sobel:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::Mat gradX, gradY;
		cv::Sobel(srcImage, gradX, CV_16S, 1, 0, kernelSize, 1, 1, cv::BORDER_DEFAULT);
		cv::Sobel(srcImage, gradY, CV_16S, 0, 1, kernelSize, 1, 1, cv::BORDER_DEFAULT);
		cv::convertScaleAbs(gradX, gradX);
		cv::convertScaleAbs(gradY, gradY);
		addWeighted(gradX, 0.5, gradY, 0.5, 0, desImage);
	}
	break;
	case Functions::Laplacian:
	{
		C_UINT32 kernelSize = atoi(argv[4]);
		cv::Laplacian(srcImage, desImage, CV_16S, 3, kernelSize, 0, cv::BORDER_DEFAULT);
		cv::convertScaleAbs(desImage, desImage);
	}
	break;
	case Functions::equalizeHist:
	{
		CVC::EqualizeHist(srcImage, desImage);
	}
	break;
	case Functions::Laplace:
	{
		cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		cv::filter2D(srcImage, desImage, srcImage.depth(), kernel);
	}
	break;
	case Functions::LogTransformation:
	{
		C_DOUBLE c = atof(argv[4]);
		CVC::LogTransformation(srcImage, desImage, c);
	}
	break;
	case Functions::GammaCorrection:
	{
		C_DOUBLE gamma = atof(argv[4]);
		CVC::GammaCorrection(srcImage, desImage, gamma);
	}
	break;

	case Functions::HistVisualization:
	{
		C_UINT32 w = atoi(argv[4]);
		C_UINT32 h = atoi(argv[5]);
		CVC::HistVisualization(srcImage, desImage, w, h);
	}
	break;
	case Functions::flip:
	{
		int typeflipCode = atoi(argv[4]);
		cv::flip(srcImage, desImage, typeflipCode);
	}
	break;
	case Functions::resize:
	{
		C_UINT32 w = atoi(argv[4]);
		C_UINT32 h = atoi(argv[5]);
		int type = atoi(argv[6]);
		cv::resize(srcImage, desImage, cv::Size(w, h), 0, 0, type);
	}
	break;

	case Functions::rotate:
	{
		C_DOUBLE angle = atof(argv[4]);
		CVC::Rotate(srcImage, desImage, angle);
	}
	break;
	case Functions::translate:
	{
		C_FLOAT x = (float)atof(argv[4]);
		C_FLOAT y = (float)atof(argv[5]);
		CVC::Translate(srcImage, desImage, x, y);
	}
	break;
	case Functions::OriginalLBP:
	{
		CVC::OriginalLBP(srcImage, desImage);
	}
	break;
	case Functions::CircularLBP:
	{
		C_UINT32 radius = atoi(argv[4]);
		CVC::CircularLBP(srcImage, desImage, radius);
	}
	break;
	case Functions::InvariantLBP:
	{
		C_UINT32 radius = atoi(argv[4]);
		CVC::InvariantLBP(srcImage, desImage, radius);
	}
	break;
	case Functions::EquivalentLBP:
	{
		C_UINT32 radius = atoi(argv[4]);
		CVC::EquivalentLBP(srcImage, desImage, radius);
	}
	break;
	case Functions::MultiScaleBlockLBP:
	{
		C_UINT32 scale = atoi(argv[4]);
		CVC::MultiScaleBlockLBP(srcImage, desImage, scale);
	}
	break;
	case Functions::SEMultiScaleBlockLBP:
	{
		C_UINT32 scale = atoi(argv[4]);
		CVC::SEMultiScaleBlockLBP(srcImage, desImage, scale);
	}
	break;
	case Functions::HOG:
	{
		C_UINT32 cell = atoi(argv[4]);
		C_UINT32 block = cell * 2;
		CVC::HOGVisualization(srcImage, desImage, cell, block);
	}
	break;
	case Functions::felzenszwalb:
	{
		C_DOUBLE sigma = atof(argv[4]);
		C_FLOAT k = (float)atof(argv[5]);
		C_INT32 minSize = atoi(argv[6]);
		CVC::felzenszwalb(srcImage, desImage, sigma, k, minSize);
	}
	break;
	case Functions::SelectiveSearch:
	{
		C_FLOAT sigma = (float)atof(argv[4]);
		C_INT32 baseK = atoi(argv[5]);
		C_INT32 incK = atoi(argv[6]);
		bool fast = true;
		CVC::SelectiveSearch(srcImage, desImage, sigma, baseK, incK, fast);
	}
	break;
	case Functions::HoughLinesP:
	{
		C_INT32 threshold = atoi(argv[4]);
		C_DOUBLE minLineLength = atof(argv[5]);
		CVC::HoughLinesP(srcImage, desImage, threshold, minLineLength);
	}
	break;
	case Functions::HoughCircles:
	{
		C_DOUBLE param2 = atof(argv[4]);
		C_INT32 minRadius = atoi(argv[5]);
		C_INT32 maxRadius = atoi(argv[4]);
		CVC::HoughCircles(srcImage, desImage, param2, minRadius, maxRadius);
	}
	break;

	}


	cv::imwrite(path + _fuctions[commend] + "_" + filename, desImage);
	return 0;
}


//int main(int argc, char *argv[])
//{
//	if (argc == 1) return 0;
//
//	int result = Commend(argv);
//	std::cout << result;
//	return result;
//}

void PlotBox(cv::Mat& img, const std::vector<int>& bbox, const std::string& text, const cv::Scalar& color = cv::Scalar(0, 255, 0))
{
	cv::rectangle(img, cv::Point(bbox[0], bbox[1]), cv::Point(bbox[2], bbox[3]), color, 2);
	cv::rectangle(img, cv::Point(bbox[0], bbox[1] - 20), cv::Point(bbox[0] + 40, bbox[1]), color, 2);
	cv::putText(img, text, cv::Point(bbox[0] + 5, bbox[1] - 5), cv::FONT_HERSHEY_PLAIN, 1, color, 2, cv::LINE_AA);
}

void PlotBox(cv::Mat& img, const cv::Rect_<float>& bbox, const std::string& text, const cv::Scalar& color = cv::Scalar(0, 255, 0))
{
	cv::rectangle(img, bbox, color, 2);
	cv::rectangle(img, cv::Point(bbox.x, bbox.y - 20), cv::Point(bbox.x + 40, bbox.y), color, 2);
	cv::putText(img, text, cv::Point(bbox.x + 5, bbox.y - 5), cv::FONT_HERSHEY_PLAIN, 1, color, 2, cv::LINE_AA);
}

std::vector<int> ConvertVector(const std::string& str)
{
	std::vector<int> bbox;
	std::string num = "";
	for (int index = 0; index < str.length(); index++)
	{
		if (str[index] == ' ')
		{
			bbox.push_back(stoi(num));
			num = "";
		}
		else
		{
			num += str[index];
		}
	}

	if (num.length() > 0) bbox.push_back(stoi(num));
	return bbox;
}

cv::Rect_<float> ConvertRect_(const std::string& str)
{
	std::vector<int> bbox = ConvertVector(str);
	float x = static_cast<float>(bbox[0]);
	float y = static_cast<float>(bbox[1]);
	float w = static_cast<float>(bbox[2] - bbox[0]);
	float h = static_cast<float>(bbox[3] - bbox[1]);
	return cv::Rect_<float>(x, y, w, h);
}


std::vector<std::vector<int>> ReadLabel2Vector(C_UINT32 frameIndex)
{
	std::vector<std::vector<int>> bboxs;
	std::ifstream ifs("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/S2L1/" + std::to_string(frameIndex) + ".txt", ifstream::in);
	std::string str;
	while (std::getline(ifs, str))
	{
		std::vector<int> bbox = ConvertVector(str);
		bboxs.push_back(bbox);
	}
	ifs.close();

	return bboxs;
}

std::vector<cv::Rect_<float>> ReadLabel2Rect_(C_UINT32 frameIndex)
{
	std::vector<cv::Rect_<float>> bboxs;
	std::ifstream ifs("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/S2L1/" + std::to_string(frameIndex) + ".txt", ifstream::in);
	std::string str;
	while (std::getline(ifs, str))
	{
		cv::Rect_<float> bbox = ConvertRect_(str);
		bboxs.push_back(bbox);
	}
	ifs.close();

	return bboxs;
}

typedef struct TrackingBox
{
	int frame;
	int id;
	cv::Rect_<float> box;
} TrackingBox;

double GetIOU(const Rect_<float>& bb_test, const Rect_<float>& bb_gt)
{
	float in = (bb_test & bb_gt).area();
	float un = bb_test.area() + bb_gt.area() - in;

	if (un < DBL_EPSILON)
		return 0;

	return (double)(in / un);
}

void TestSORT()
{
	UINT32 maxAge = 1;
	UINT32 minHits = 3;
	UINT32 index = 0;
	double iouThreshold = 0.3;
	UINT32 trkNum = 0;
	UINT32 detNum = 0;
	std::vector<KalmanTracker> trackers;
	cv::Mat frame;
	cv::VideoCapture cap = cv::VideoCapture("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/PETS09-S2L1-raw.webm");
	std::unordered_map<int, cv::Scalar> personColor;
	std::vector<Rect_<float>> predictedBoxes;
	std::vector<vector<double>> iouMatrix;
	std::vector<int> assignment;
	vector<cv::Point> matchedPairs;
	std::set<int> unmatchedDetections;
	std::set<int> allItems;
	std::set<int> matchedItems;

	vector<TrackingBox> frameTrackingResult;

	while (cap.isOpened())
	{
		cap.read(frame);
		if (frame.empty())
		{
			cerr << "ERROR! blank frame grabbed\n";
			break;
		}

		std::vector<cv::Rect_<float>>  bboxs = ReadLabel2Rect_(index);

		if (trackers.size() == 0)
		{
			for (auto bbox : bboxs)
			{
				KalmanTracker trk = KalmanTracker(bbox);
				trackers.push_back(trk);
			}
			continue;
		}

		predictedBoxes.clear();
		for (auto it = trackers.begin(); it != trackers.end();)
		{
			Rect_<float> pBox = (*it).predict();
			if (pBox.x >= 0 && pBox.y >= 0)
			{
				predictedBoxes.push_back(pBox);
				it++;
			}
			else
			{
				it = trackers.erase(it);
				//cerr << "Box invalid at frame: " << frame_count << endl;
			}
		}


		trkNum = predictedBoxes.size();
		detNum = bboxs.size();

		iouMatrix.clear();
		iouMatrix.resize(trkNum, vector<double>(detNum, 0));

		for (unsigned int i = 0; i < trkNum; i++) // compute iou matrix as a distance matrix
		{
			for (unsigned int j = 0; j < detNum; j++)
			{
				// use 1-iou because the hungarian algorithm computes a minimum-cost assignment.
				iouMatrix[i][j] = 1 - GetIOU(predictedBoxes[i], bboxs[j]);
			}
		}

		// solve the assignment problem using hungarian algorithm.
		// the resulting assignment is [track(prediction) : detection], with len=preNum
		HungarianAlgorithm HungAlgo;
		assignment.clear();
		HungAlgo.Solve(iouMatrix, assignment);




		unmatchedDetections.clear();
		allItems.clear();
		matchedItems.clear();

		if (detNum > trkNum) //	there are unmatched detections
		{
			for (unsigned int n = 0; n < detNum; n++)
				allItems.insert(n);

			for (unsigned int i = 0; i < trkNum; ++i)
				matchedItems.insert(assignment[i]);

			set_difference(allItems.begin(), allItems.end(),
				matchedItems.begin(), matchedItems.end(),
				insert_iterator<set<int>>(unmatchedDetections, unmatchedDetections.begin()));
		}

		// filter out matched with low IOU
		matchedPairs.clear();
		for (unsigned int i = 0; i < trkNum; ++i)
		{
			if (assignment[i] == -1) // pass over invalid values
				continue;
			if (1 - iouMatrix[i][assignment[i]] < iouThreshold)
			{
				unmatchedDetections.insert(assignment[i]);
			}
			else
				matchedPairs.push_back(cv::Point(i, assignment[i]));
		}


		///////////////////////////////////////
		// 3.3. updating trackers

		// update matched trackers with assigned detections.
		// each prediction is corresponding to a tracker
		int detIdx, trkIdx;
		for (unsigned int i = 0; i < matchedPairs.size(); i++)
		{
			trkIdx = matchedPairs[i].x;
			detIdx = matchedPairs[i].y;
			trackers[trkIdx].update(bboxs[detIdx]);
		}

		// create and initialise new trackers for unmatched detections
		for (auto umd : unmatchedDetections)
		{
			KalmanTracker tracker = KalmanTracker(bboxs[umd]);
			trackers.push_back(tracker);
		}

		// get trackers' output
		frameTrackingResult.clear();
		for (auto it = trackers.begin(); it != trackers.end();)
		{
			if (((*it).m_time_since_update < 1) &&
				((*it).m_hit_streak >= minHits || index <= minHits))
			{
				TrackingBox res;
				res.box = (*it).get_state();
				res.id = (*it).m_id + 1;
				res.frame = index;
				frameTrackingResult.push_back(res);
				it++;
				if (personColor.find(res.id) == personColor.end())
					personColor[res.id] =  CVC::Vec3b2Scalar(CVC::GetColor());
				PlotBox(frame, res.box, std::to_string(res.id), personColor[res.id]);
			}
			else
				it++;

			// remove dead tracklet
			if (it != trackers.end() && (*it).m_time_since_update > maxAge)
				it = trackers.erase(it);
		}
		imshow("Trick", frame);
		if (waitKey(100) >= 0)
			break;
		index++;
	}
}

int main()
{
	TestSORT();
	system("pause");
}

//int main()
//{
//	
//	UINT32 index = 0;
//	cv::Mat frame;
//	cv::VideoCapture cap = cv::VideoCapture("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/PETS09-S2L1-raw.webm");
//	std::unordered_map<int, cv::Scalar> personColor;
//	while (cap.isOpened())
//	{
//		cap.read(frame);
//		if (frame.empty()) 
//		{
//			cerr << "ERROR! blank frame grabbed\n";
//			break;
//		}
//
//		std::vector<std::vector<int>>  bboxs = ReadLabel2Vector(index++);
//		for (int index = 0; index < bboxs.size(); index++)
//		{
//			const std::vector<int>& bbox = bboxs[index];
//
//			if (personColor.find(bbox[4]) == personColor.end())
//				personColor[bbox[4]] =  CVC::Vec3b2Scalar(CVC::GetColor());
//
//			PlotBox(frame, bbox, std::to_string(bbox[4]), personColor[bbox[4]]);
//		}
//		
//		imshow("Trick", frame);
//		if (waitKey(5) >= 0)
//			break;
//	}
//	system("pause");
//}

//#include <opencv2/core/core.hpp>
//#include <opencv2/tracking/tracker.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/core/utility.hpp>
//#include <iostream>
//#include <string>
//#include <math.h>
//
//using namespace cv;
//using namespace std;
//
//// Convert to string
//#define SSTR( x ) static_cast< std::ostringstream & >( \
//( std::ostringstream() << std::dec << x ) ).str()
//
//
//Ptr<Tracker> CreateTrackerByName(const string& trackerType)
//{
//	Ptr<Tracker> tracker;
//	string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE" , "CSRT" };
//
//	if (trackerType == trackerTypes[0])
//		tracker = TrackerBoosting::create();
//	else if (trackerType == trackerTypes[1])
//		tracker = TrackerMIL::create();
//	else if (trackerType == trackerTypes[2])
//		tracker = TrackerKCF::create();
//	else if (trackerType == trackerTypes[3])
//		tracker = TrackerTLD::create();
//	else if (trackerType == trackerTypes[4])
//		tracker = TrackerMedianFlow::create();
//	else if (trackerType == trackerTypes[5])
//		tracker = TrackerGOTURN::create();
//	else if (trackerType == trackerTypes[6])
//		tracker = TrackerMOSSE::create();
//	else if (trackerType == trackerTypes[7])
//		tracker = TrackerCSRT::create();
//	else
//		tracker = TrackerKCF::create();
//	return tracker;
//}
//
//
//void GetRandomColors(vector<Scalar>& colors, int numColors)
//{
//	RNG rng(0);
//	for (int i = 0; i < numColors; i++)
//		colors.push_back(Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255)));
//}
//
//int main()
//{
//
//
//	// Read video
//	VideoCapture video("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/GOPR0058.mp4");
//	Mat frame;
//	Mat resizeframe;
//
//	// Exit if video is not opened
//
//	if (!video.isOpened())
//	{
//		cout << "Could not read video file" << endl;
//		system("pause");
//		return 1;
//
//	}
//
//	// Read first frame
//
//	bool ok = video.read(frame);
//	cv::resize(frame, resizeframe, Size(800, 480));
//	namedWindow("avc", 0);
//
//	std::vector<cv::Rect> bboxes;
//	cv::selectROIs("MultiTracker", resizeframe, bboxes, true, false);
//
//	std::vector<cv::Ptr<cv::Tracker>> multiTracker = std::vector<cv::Ptr<cv::Tracker>>(bboxes.size());
//
//	// Initialize multitracker
//	for (int i = 0; i < bboxes.size(); i++)
//	{
//		multiTracker[i] = CreateTrackerByName("KCF");
//		multiTracker[i]->init(resizeframe, static_cast<Rect2d>(bboxes[i]));
//	}
//
//	// Display bounding box.
//	std::vector<Scalar> colors;
//	GetRandomColors(colors, bboxes.size());
//
//
//
//
//	//	tracker->update(resizeframe, bbox);
//	while (video.read(frame))
//	{
//
//		cv::resize(frame, resizeframe, Size(800, 480));
//		// Start timer
//		double timer = (double)getTickCount();
//
//		
//		// Update the tracking result
//		for (int i = 0; i < bboxes.size(); i++)
//		{
//			Rect2d& rect = static_cast<Rect2d>(bboxes[i]);
//			bool sucess = multiTracker[i]->update(resizeframe, rect);
//			if (sucess)
//			{
//				rectangle(resizeframe, rect, colors[i], 2, 1);
//			}
//
//
//		}
//
//		float fps = getTickFrequency() / ((double)getTickCount() - timer);
//
//		// Display FPS on frame
//		putText(resizeframe, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//
//		// Display frame.
//		imshow("Tracking", resizeframe);
//
//		// Exit if ESC pressed.
//		int k = waitKey(1);
//		if (k == 27)
//		{
//			break;
//		}
//
//	}
//}




//int main()
//{
//
//	// List of tracker types in OpenCV 3.2
//	// NOTE : GOTURN implementation is buggy and does not work.
//	string trackerTypes[8] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE" , "CSRT" };
//	// vector <string> trackerTypes(types, std::end(types));
//
//	// Create a tracker
//	string trackerType = trackerTypes[2]; // you can select the tracking type here. 
//
//	Ptr<Tracker> tracker;
//
//	if (trackerType == "BOOSTING") 
//		tracker = TrackerBoosting::create(); 
//	if (trackerType == "MIL") 
//		tracker = TrackerMIL::create(); 
//	if (trackerType == "KCF") 
//		tracker = TrackerKCF::create(); 
//	if (trackerType == "TLD") 
//		tracker = TrackerTLD::create(); 
//	if (trackerType == "MEDIANFLOW") 
//		tracker = TrackerMedianFlow::create(); 
//	if (trackerType == "GOTURN") 
//		tracker = TrackerGOTURN::create();
//	if (trackerType == "MOSSE")
//		tracker = TrackerMOSSE::create(); 
//	if (trackerType == "CSRT") 
//		tracker = TrackerCSRT::create();
//
//	// Read video
//	VideoCapture video("D:/VisualStudio/VisualStudioProject/CVC/x64/Debug/GOPR0058.mp4");
//	Mat frame;
//	Mat resizeframe;
//
//	// Exit if video is not opened
//
//	if (!video.isOpened())
//	{
//		cout << "Could not read video file" << endl;
//		system("pause");
//		return 1;
//
//	}
//
//	// Read first frame
//
//	bool ok = video.read(frame);
//	cv::resize(frame, resizeframe, Size(800, 480));
//	namedWindow("avc", 0);
//	// Define initial boundibg box
//	Rect2d bbox(287, 23, 86, 320);
//
//	// Uncomment the line below to select a different bounding box
//	bbox = selectROI(resizeframe, false);
//
//	// Display bounding box.
//	rectangle(resizeframe, bbox, Scalar(255, 0, 0), 2, 1);
//	imshow("Tracking", resizeframe);
//
//	tracker->init(resizeframe, bbox);
//
//	
//	//	tracker->update(resizeframe, bbox);
//	while (video.read(frame))
//	{
//
//		cv::resize(frame, resizeframe, Size(800, 480));
//		// Start timer
//		double timer = (double)getTickCount();
//
//		// Update the tracking result
//		bool ok = tracker->update(resizeframe, bbox);
//
//		// Calculate Frames per second (FPS)
//		float fps = getTickFrequency() / ((double)getTickCount() - timer);
//
//		if (ok)
//		{
//			// Tracking success : Draw the tracked object
//			rectangle(resizeframe, bbox, Scalar(255, 0, 0), 2, 1);
//		}
//		else
//		{
//			// Tracking failure detected.
//			putText(resizeframe, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
//		}
//
//		// Display tracker type on frame
//		putText(resizeframe, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//
//		// Display FPS on frame
//		putText(resizeframe, "FPS : " + SSTR(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
//
//		// Display frame.
//		imshow("Tracking", resizeframe);
//
//		// Exit if ESC pressed.
//		int k = waitKey(1);
//		if (k == 27)
//		{
//			break;
//		}
//
//	}
//}