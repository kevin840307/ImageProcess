#include "Library.h"


namespace IVC {

	void ChangeColor(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 type)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);

		switch (type)
		{
		case ColerType::BGR2GRAY_8BIT:
			Inner::BGR2Gray8Bit(src, pur, width, height);
			break;
			//case ColerType::BGR2HSV:
			//	Inner::BGR2HSV(src, pur, width, height);
			//	break;
			//case ColerType::HSV2BGR:
			//	Inner::HSV2BGR(src, pur, width, height);
			//	break;
			//case ColerType::BGR2YCbCr:
			//	Inner::BGR2YCbCr(src, pur, width, height);
			//	break;
			//case ColerType::YCbCr2BGR:
			//	Inner::YCbCr2BGR(src, pur, width, height);
			//	break;
		}
	}

	void ImagePadding8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_INT32 pad)
	{
		assert(src != nullptr && pur != nullptr);
		assert(width > 0 && height > 0);
		assert(pad >= 0);

		C_UINT32 copySize = width * sizeof(UCHAR);
		C_UINT32 purWidth = (width + (pad << 1));
		C_UCHAR* srcEnd = src + width * height;

		pur += (pad * purWidth) + pad;

		while (src < srcEnd)
		{
			memcpy(pur, src, copySize);
			pur += purWidth;
			src += width;
		}
	}

	void Resize8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_UINT32 reWidth, C_UINT32 reHeight
		, ResizeType type)
	{
		switch (type)
		{
		case ResizeType::NEAREST:
			Inner::NearestResize8bit(src, pur
				, width, height
				, reWidth, reHeight);
			break;
		case ResizeType::LINEAR:
			Inner::LinearResize8bit(src, pur
				, width, height
				, reWidth, reHeight);
			break;
		case ResizeType::BICUBIC:
			Inner::BiCubicResize8bit(src, pur
				, width, height
				, reWidth, reHeight);
			break;
		}
	}

	void ContourFrame8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height)
	{
		Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, ImageType::GRAY_8BIT);

		vector<int> links(1, 0);
		vector<vector<int>> mark(height, vector<int>(width, 0));
		int linkNum = 1;

		for (int col = 1; col < height - 1; col++)
		{
			for (int row = 1; row < width - 1; row++)
			{
				if (srcImage.image[col][row] != 255)
				{
					continue;
				}

				vector<int> link;
				if (mark[col - 1][row - 1] != 0)
				{
					link.push_back(mark[col - 1][row - 1]);
				}

				if (mark[col - 1][row] != 0)
				{
					link.push_back(mark[col - 1][row]);
				}

				if (mark[col - 1][row + 1] != 0)
				{
					link.push_back(mark[col - 1][row + 1]);
				}

				if (mark[col][row - 1] != 0)
				{
					link.push_back(mark[col][row - 1]);
				}

				if (link.empty())
				{
					mark[col][row] = linkNum;
					links.push_back(linkNum);
					linkNum++;
				}
				else
				{
					sort(link.begin(), link.end());
					mark[col][row] = links[link[0]];
					for (int index = 1; index < link.size(); index++)
					{
						//和原本的最小等價比較
						if (links[link[0]] < links[link[index]])
						{
							links[link[index]] = links[link[0]];
						}
						else if (links[link[0]] > links[link[index]])
						{
							links[link[0]] = links[link[index]];
						}
					}
				}

			}
		}

		// 修正權重
		set<int> setLink;
		for (int index = 1; index < links.size(); index++)
		{
			int saveValue = links[index];
			int saveIndex = index;
			while (saveValue != saveIndex)
			{
				saveValue = links[links[saveIndex]];
				saveIndex = links[saveIndex];
			}
			links[index] = saveValue;
			setLink.insert(saveValue);
		}

		map<int, int> mapLink;
		int save = 1;
		for (set<int>::iterator iter = setLink.begin(); iter != setLink.end(); iter++)
		{
			mapLink[*iter] = save++;
		}

		for (int col = 0; col < height; col++)
		{
			for (int row = 0; row < width; row++)
			{
				purImage.image[col][row] = mapLink[mark[col][row]] * 15;
			}
		}
	}


	void PerspectiveTransformReverse8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_FLOAT* points
		, PerspectiveTransType type)
	{
		Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, ImageType::GRAY_8BIT);
		int fixHeight = height >> 2;
		int fixWidth = width >> 2;
		for (UINT32 col = 0; col < 4; col++)
		{
			int fixColMin = fixHeight * col;
			int fixColMax = fixHeight * col + fixHeight;

			for (UINT32 row = 0; row < 4; row++)
			{
				int fixRowMin = fixWidth * row;
				int fixRowMax = fixWidth * row + fixWidth;
				int rowOffset = row * 2;
				int x0 = *(points + 10 * col + rowOffset);
				int x1 = *(points + 10 * col + rowOffset + 2);
				int x2 = *(points + 10 * (col + 1) + rowOffset);
				int x3 = *(points + 10 * (col + 1) + rowOffset + 2);

				int y0 = *(points + 10 * col + 1 + rowOffset);
				int y1 = *(points + 10 * col + 3 + rowOffset);
				int y2 = *(points + 10 * (col + 1) + 1 + rowOffset);
				int y3 = *(points + 10 * (col + 1) + 3 + rowOffset);

				PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
					fixRowMin, fixColMin,
					fixRowMax, fixColMin,
					fixRowMin, fixColMax,
					fixRowMax, fixColMax,
					x0, y0,
					x1, y1,
					x2, y2,
					x3, y3);


				vector<float> ponits;

				for (int i = fixColMin; i < fixColMax; i++) {
					for (int j = fixRowMin; j < fixRowMax; j++) {
						ponits.push_back(j);
						ponits.push_back(i);
					}
				}

				tansform.transformPoints(ponits);

				switch (type)
				{
				case PerspectiveTransType::TRANS_NEAREST:
					Inner::PerspectiveTransformNearReverse8bit(srcImage, purImage
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				case PerspectiveTransType::TRANS_LINEAR:
					Inner::PerspectiveTransformLinearReverse8bit(srcImage, purImage
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				case PerspectiveTransType::TRANS_BICUBIC:
					Inner::PerspectiveTransformBiCubicReverse8bit(srcImage, purImage
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				}
			}
		}
	}

	void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
		, C_UINT32 width, C_UINT32 height
		, C_FLOAT* points
		, PerspectiveTransType type)
	{
		Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
		Image purImage(pur, width, height, ImageType::GRAY_8BIT);

		//for (UINT32 row = 0; row < height; row++)
		//{
		//	memcpy(purImage.image[row], srcImage.image[row], width * sizeof(UCHAR));
		//}

		int fixHeight = height >> 2;
		int fixWidth = width >> 2;
		for (UINT32 col = 0; col < 4; col++)
		{
			int fixColMin = fixHeight * col;
			int fixColMax = fixHeight * col + fixHeight;

			for (UINT32 row = 0; row < 4; row++)
			{
				int fixRowMin = fixWidth * row;
				int fixRowMax = fixWidth * row + fixWidth;
				int rowOffset = row * 2;
				int x0 = *(points + 10 * col + rowOffset);
				int x1 = *(points + 10 * col + rowOffset + 2);
				int x2 = *(points + 10 * (col + 1) + rowOffset);
				int x3 = *(points + 10 * (col + 1) + rowOffset + 2);

				int y0 = *(points + 10 * col + 1 + rowOffset);
				int y1 = *(points + 10 * col + 3 + rowOffset);
				int y2 = *(points + 10 * (col + 1) + 1 + rowOffset);
				int y3 = *(points + 10 * (col + 1) + 3 + rowOffset);

				PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
					x0, y0,
					x1, y1,
					x2, y2,
					x3, y3,
					fixRowMin, fixColMin, // top left
					fixRowMax, fixColMin, // top right
					fixRowMin, fixColMax, // bottom left
					fixRowMax, fixColMax  // bottom right
				);


				vector<float> ponits;
				int minX = min(min(min(x0, x1), x2), x3);
				int maxX = max(max(max(x0, x1), x2), x3);
				int minY = min(min(min(y0, y1), y2), y3);
				int maxY = max(max(max(y0, y1), y2), y3);

				for (int y = minY; y < maxY; y++) {
					for (int x = minX; x < maxX; x++) {
						ponits.push_back(x);
						ponits.push_back(y);
					}
				}

				tansform.transformPoints(ponits);

				switch (type)
				{
				case PerspectiveTransType::TRANS_NEAREST:
					Inner::PerspectiveTransformNear8bit(srcImage, purImage
						, minX, minY
						, maxX, maxY
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				case PerspectiveTransType::TRANS_LINEAR:
					Inner::PerspectiveTransformLinear8bit(srcImage, purImage
						, minX, minY
						, maxX, maxY
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				case PerspectiveTransType::TRANS_BICUBIC:
					Inner::PerspectiveTransformBiCubic8bit(srcImage, purImage
						, minX, minY
						, maxX, maxY
						, fixRowMin, fixRowMax
						, fixColMin, fixColMax
						, x0, y0
						, x1, y1
						, x2, y2
						, x3, y3
						, ponits);
					break;
				}
			}
		}
	}



	// can run var
	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur, C_UCHAR* grid
	//	, C_UINT32 width, C_UINT32 height
	//	, C_FLOAT* points)
	//{
	//	Image gridImage(const_cast<UCHAR*>(grid), width, height, ImageType::GRAY_8BIT);
	//	//for (int col = 0; col < height; col++)
	//	//{
	//	//	for (int row = 0; row< width; row++)
	//	//	{
	//	//		gridImage.image[col][row] = 255;
	//	//	}
	//	//}
	//	//for (UINT32 col = 0; col < 5; col++)
	//	//{
	//	//	for (UINT32 row = 0; row < 5; row++)
	//	//	{
	//	//		int newRow = row * 2;
	//	//		int x = *(points + 10 * col + newRow);
	//	//		int y = *(points + 10 * col + 1 + newRow);

	//	//		Point point(x, y);
	//	//		if (row < 4)
	//	//		{
	//	//			int rightX = *(points + 10 * col + newRow + 2) ;
	//	//			int rightY = *(points + 10 * col + newRow + 3) ;

	//	//			Point rightPoint(rightX, rightY);
	//	//			DrawLine8bit(gridImage, point, rightPoint, 0);
	//	//		}

	//	//		if (col < 4)
	//	//		{
	//	//			int bottomX = *(points + 10 * (col + 1) + newRow);
	//	//			int bottomY = *(points + 10 * (col + 1) + 1 + newRow);
	//	//			Point bottomPoint(bottomX, bottomY);
	//	//			DrawLine8bit(gridImage, point, bottomPoint, 0);
	//	//		}
	//	//	}
	//	//}

	//	UCHAR* fillData = new UCHAR[width * height]{ 0 };
	//	Image fillImage(fillData, width, height, ImageType::GRAY_8BIT);
	//	ContourFrame8bit(grid, fillData, width, height);


	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);

	//	for (UINT32 col = 0; col < 4; col++)
	//	{
	//		int fixCol = 79 * col;
	//		int fixCol2 = 79 * col + 79;

	//		for (UINT32 row = 0; row < 4; row++)
	//		{
	//			int fixRow = 79 * row;
	//			int fixRow2 = 79 * row + 79;
	//			int newRow = row * 2;
	//			int x0 = *(points + 10 * col + newRow);
	//			int x1 = *(points + 10 * col + newRow + 2);
	//			int x2 = *(points + 10 * (col + 1) + newRow);
	//			int x3 = *(points + 10 * (col + 1) + newRow + 2);

	//			int y0 = *(points + 10 * col + 1 + newRow);
	//			int y1 = *(points + 10 * col + 3 + newRow);
	//			int y2 = *(points + 10 * (col + 1) + 1 + newRow);
	//			int y3 = *(points + 10 * (col + 1) + 3 + newRow);
	//			int minX = min(min(min(x0, x1), x2), x3);
	//			int maxX = max(max(max(x0, x1), x2), x3);
	//			int minY = min(min(min(y0, y1), y2), y3);
	//			int maxY = max(max(max(y0, y1), y2), y3);

	//			PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//				x0, y0,
	//				x1, y1,
	//				x2, y2,
	//				x3, y3,
	//				fixRow, fixCol, // top left
	//				fixRow2, fixCol, // top right
	//				fixRow, fixCol2,// bottom left
	//				fixRow2, fixCol2
	//			);


	//			vector<float> ponits;
	//			for (int i = minY; i < maxY; i++) {
	//				for (int j = minX; j < maxX; j++) {
	//					ponits.push_back(j);
	//					ponits.push_back(i);
	//				}
	//			}
	//			int grayX = (x0 + x1 + x2 + x3) >> 2;
	//			int grayY = (y0 + y1 + y2 + y3) >> 2;
	//			int gray = fillImage.image[grayY][grayX];
	//			tansform.transformPoints(ponits);
	//			for (int i = minY; i < maxY; i++) {
	//				for (int j = minX; j < maxX; j++) {
	//					if(gray != fillImage.image[i][j] && fillImage.image[i][j] != 0) continue;
	//					int tmp = (i - minY) * (maxX - minX) + (j - minX);
	//					if (tmp < 0 || ponits.size() - 1 < tmp * 2 + 1) continue;
	//					int x = ponits[tmp * 2];
	//					int y = ponits[tmp * 2 + 1];

	//					if (x<fixRow || x>(fixRow2 - 1) || y<fixCol || y>(fixCol2 - 1))
	//						continue;
	//					purImage.image[i][j] = srcImage.image[y][x];
	//				}
	//			}
	//		}
	//	}

	//	delete[] fillData;
	//}

	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height
	//	, C_FLOAT* points)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);

	//	for (UINT32 col = 0; col < 4; col++)
	//	{
	//		int fixCol = 79 * col;

	//		for (UINT32 row = 0; row < 4; row++)
	//		{
	//			int fixRow = 79 * row;
	//			int newRow = row * 2;
	//			int x0 = *(points + 10 * col + newRow) - fixRow;
	//			int x1 = *(points + 10 * col + newRow + 2) - fixRow;
	//			int x2 = *(points + 10 * (col + 1) + newRow) - fixRow;
	//			int x3 = *(points + 10 * (col + 1) + newRow + 2) - fixRow;

	//			int y0 = *(points + 10 * col + 1 + newRow) - fixCol;
	//			int y1 = *(points + 10 * col + 3 + newRow) - fixCol;
	//			int y2 = *(points + 10 * (col + 1) + 1 + newRow) - fixCol;
	//			int y3 = *(points + 10 * (col + 1) + 3 + newRow) - fixCol;
	//			int minX = min(min(min(x0, x1), x2), x3);
	//			int minY = min(min(min(y0, y1), y2), y3);
	//			int maxX = max(max(max(x0, x1), x2), x3);
	//			int maxY = max(max(max(y0, y1), y2), y3);
	//			PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//				x0, y0,
	//				x1, y1,
	//				x2, y2,
	//				x3, y3,
	//				0, 0, // top left
	//				79, 0, // top right
	//				0, 79,// bottom left
	//				79, 79
	//			);


	//			vector<float> ponits;
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					ponits.push_back(j);
	//					ponits.push_back(i);
	//				}
	//			}

	//			tansform.transformPoints(ponits);
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					int tmp = i * 79 + j;
	//					int x = ponits[tmp * 2];
	//					int y = ponits[tmp * 2 + 1];

	//					if (x<0 || x>(79 - 1) || y<0 || y>(79 - 1))
	//						continue;
	//					purImage.image[i + fixCol][j + fixRow] = srcImage.image[y + fixCol][x + fixRow];
	//				}
	//			}
	//		}
	//	}
	//}

	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height
	//	, C_FLOAT* points)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);

	//	for (UINT32 col = 0; col < 4; col++)
	//	{
	//		int fixCol = 79 * col;

	//		for (UINT32 row = 0; row < 4; row++)
	//		{
	//			int fixRow = 79 * row;
	//			int newRow = row * 2;
	//			int x0 = *(points + 10 * col + newRow) - fixRow;
	//			int x1 = *(points + 10 * col + newRow + 2) - fixRow;
	//			int x2 = *(points + 10 * (col + 1) + newRow) - fixRow;
	//			int x3 = *(points + 10 * (col + 1) + newRow + 2) - fixRow;

	//			int y0 = *(points + 10 * col + 1 + newRow) - fixCol;
	//			int y1 = *(points + 10 * col + 3 + newRow) - fixCol;
	//			int y2 = *(points + 10 * (col + 1) + 1 + newRow) - fixCol;
	//			int y3 = *(points + 10 * (col + 1) + 3 + newRow) - fixCol;
	//			int minX = min(min(min(x0, x1),x2),x3);
	//			int minY = min(min(min(y0, y1), y2), y3);
	//			int maxX = max(max(max(x0, x1), x2), x3);
	//			int maxY = max(max(max(y0, y1), y2), y3);
	//			PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//				x0, y0,
	//				x1, y1,
	//				x2, y2,
	//				x3, y3,
	//				0, 0, // top left
	//				79, 0, // top right
	//				0, 79,// bottom left
	//				79, 79
	//			);


	//			vector<float> ponits;
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					ponits.push_back(j);
	//					ponits.push_back(i);
	//				}
	//			}

	//			tansform.transformPoints(ponits);
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					int tmp = i * 79 + j;
	//					int x = ponits[tmp * 2];
	//					int y = ponits[tmp * 2 + 1];

	//					if (x<0 || x>(79 - 1) || y<0 || y>(79 - 1))
	//						continue;
	//					purImage.image[i + fixCol][j + fixRow] = srcImage.image[y + fixCol][x + fixRow];
	//				}
	//			}
	//		}
	//	}
	//}


	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height
	//	, C_FLOAT* points)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);

	//	for (UINT32 col = 0; col < 4; col++)
	//	{
	//		int fixCol = 79 * col;

	//		for (UINT32 row = 0; row < 4; row++)
	//		{
	//			int fixRow = 79 * row;
	//			int newRow = row * 2;
	//			int x0 = *(points + 10 * col + newRow) - fixRow;
	//			int x1 = *(points + 10 * col + newRow + 2) - fixRow;
	//			int x2 = *(points + 10 * (col + 1) + newRow) - fixRow;
	//			int x3 = *(points + 10 * (col + 1) + newRow + 2) - fixRow;

	//			int y0 = *(points + 10 * col + 1 + newRow) - fixCol;
	//			int y1 = *(points + 10 * col + 3 + newRow) - fixCol;
	//			int y2 = *(points + 10 * (col + 1) + 1 + newRow) - fixCol;
	//			int y3 = *(points + 10 * (col + 1) + 3 + newRow) - fixCol;

	//			PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//				x0, y0,
	//				x1, y1,
	//				x2, y2,
	//				x3, y3,
	//				0, 0, // top left
	//				79, 0, // top right
	//				0, 79,// bottom left
	//				79, 79
	//			);


	//			vector<float> ponits;
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					ponits.push_back(j);
	//					ponits.push_back(i);
	//				}
	//			}

	//			tansform.transformPoints(ponits);
	//			for (int i = 0; i < 79; i++) {
	//				for (int j = 0; j < 79; j++) {
	//					int tmp = i * 79 + j;
	//					int x = ponits[tmp * 2];
	//					int y = ponits[tmp * 2 + 1];
	//					if (x<0 || x>(79 - 1) || y<0 || y>(79 - 1))
	//						continue;
	//					purImage.image[i + fixCol][j + fixRow] = srcImage.image[y + fixCol][x + fixRow];
	//				}
	//			}
	//		}
	//	}
	//}

	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);
	//	int img_height = height;
	//	int img_width = width;
	//	int gideWidth = width >> 2 - 1;
	//	int gideHeight= height >> 2 - 1;

	//	PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//		0, 0, // left top
	//		gideWidth, 0,
	//		0, gideHeight,
	//		gideWidth + 50, gideHeight + 20,
	//		0, 0,
	//		gideWidth, 0, // top right
	//		0, gideHeight,// bottom left
	//		gideWidth, gideHeight
	//	);
	//	//int fixGridWidth = (gideWidth << 1) - 1;
	//	//int fixGridHeight = (gideHeight << 1) - 1;
	//	//PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//	//	gideWidth, 50, // left top
	//	//	fixGridWidth, gideHeight,
	//	//	gideWidth, fixGridHeight,
	//	//	fixGridWidth, fixGridHeight,
	//	//	gideWidth, gideHeight,
	//	//	fixGridWidth, gideHeight, // top right
	//	//	gideWidth, fixGridHeight,// bottom left
	//	//	fixGridWidth, fixGridHeight
	//	//);



	//	vector<float> ponits;
	//	for (int i = 0; i < height; i++) {
	//		for (int j = 0; j < width; j++) {
	//			ponits.push_back(j);
	//			ponits.push_back(i);
	//		}
	//	}
	//	tansform.transformPoints(ponits);
	//	for (int i = 0; i < height; i++) {
	//		for (int j = 0; j < width; j++) {
	//			int tmp = i*width + j;
	//			int x = ponits[tmp * 2];
	//			int y = ponits[tmp * 2 + 1];
	//			if (x<0 || x>(width - 1) || y<0 || y>(height - 1))
	//				continue;
	//			purImage.image[i][j] = srcImage.image[y][x];
	//		}
	//	}
	//}

	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);
	//	//int xStart = 130;
	//	//int yStart = 130;

	//	int gridHeight = 79;
	//	int gridWidth = 79;

	//	//PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//	//	0, 0,
	//	//	gridWidth - 1, 0,
	//	//	0, gridHeight - 1,
	//	//	gridWidth - 1, gridHeight - 1,
	//	//	20, 50, // top left
	//	//	gridWidth - 1, 0, // top right
	//	//	0, gridHeight - 1,// bottom left
	//	//	gridWidth - 1, gridHeight - 1
	//	//);

	//	PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//		25, 50,
	//		gridWidth - 1, 0,
	//		0, gridHeight - 1,
	//		gridWidth - 1, gridHeight - 1,
	//		0, 0, // top left
	//		gridWidth - 1, 0, // top right
	//		0, gridHeight - 1,// bottom left
	//		gridWidth - 1, gridHeight - 1
	//	);

	//	vector<float> ponits;
	//	for (int i = 0; i < gridHeight; i++) {
	//		for (int j = 0; j < gridWidth; j++) {
	//			ponits.push_back(j);
	//			ponits.push_back(i);
	//		}
	//	}
	//	tansform.transformPoints(ponits);

	//	for (int i = 0; i < gridHeight; i++) {
	//		for (int j = 0; j < gridWidth; j++) {
	//			int tmp = i*gridWidth + j;
	//			int x = ponits[tmp * 2];
	//			int y = ponits[tmp * 2 + 1];
	//			if (x<0 || x>(gridWidth) || y<0 || y>(gridHeight))
	//			{
	//				int i = 0;
	//				continue;
	//			}
	//				
	//			purImage.image[i][j] = srcImage.image[y][x];
	//		}
	//	}


	//}

	//void PerspectiveTransform8bit(C_UCHAR* src, UCHAR* pur
	//	, C_UINT32 width, C_UINT32 height)
	//{
	//	Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
	//	Image purImage(pur, width, height, ImageType::GRAY_8BIT);
	//	int img_height = height;
	//	int img_width = width;

	//	PerspectiveTransform tansform = PerspectiveTransform::quadrilateralToQuadrilateral(
	//		30, 30,
	//		img_width - 1, 0,
	//		0, img_height - 1,
	//		img_width - 1, img_height - 1,
	//		0, 0, // top left
	//		img_width - 1, 0, // top right
	//		0, img_height - 1,// bottom left
	//		img_width - 1, img_height - 1
	//	);


	//	vector<float> ponits;
	//	for (int i = 0; i < img_height; i++) {
	//		for (int j = 0; j < img_width; j++) {
	//			ponits.push_back(j);
	//			ponits.push_back(i);
	//		}
	//	}
	//	tansform.transformPoints(ponits);
	//	for (int i = 0; i < img_height; i++) {
	//		for (int j = 0; j < img_width; j++) {
	//			int tmp = i*img_width + j;
	//			int x = ponits[tmp * 2];
	//			int y = ponits[tmp * 2 + 1];
	//			if (x<0 || x>(img_width - 1) || y<0 || y>(img_height - 1))
	//				continue;
	//			purImage.image[i][j] = srcImage.image[y][x];
	//		}
	//	}
	//}
}

namespace IVC {
	namespace Inner {
		// 灰階
		// R 0.299 ≈ 19595
		// G 0.587 ≈ 38469
		// B 0.114 ≈ 7472 (進位 反推回去比7471接近)
		// 為了快速運算(整數運算+位移) 先將數值左位移16次冪 65536
		void Inner::BGR2Gray8Bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height)
		{
			Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::BGR_24BIT);
			Image purImage(pur, width, height, ImageType::GRAY_8BIT);

			for (UINT32 row = 0; row < height; row++)
			{
				for (UINT32 col = 0; col < width; col++)
				{
					Pixel pixel = srcImage.GetPixel(row, col);
					C_UCHAR pix = static_cast<UCHAR>((pixel.R * 19595 + pixel.G * 38469 + pixel.B * 7472) >> 16);
					purImage.image[row][col] = pix;
				}
			}

			/*C_UCHAR* purEnd = pur + width * height;

			while (pur < purEnd)
			{
				C_UINT32 B = *src;
				src++;

				C_UINT32 G = *src;
				src++;

				C_UINT32 R = *src;
				src++;

				C_UCHAR pix = static_cast<UCHAR>((R * 19595 + G * 38469 + B * 7472) >> 16);
				*pur = pix;
				pur++;
			}*/

		}

		void Inner::NearestResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight)
		{
			Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
			Image purImage(pur, reWidth, reHeight, ImageType::GRAY_8BIT);

			float xBase = static_cast<float>(width - 1) / static_cast<float>(reWidth - 1);
			float yBase = static_cast<float>(height - 1) / static_cast<float>(reHeight - 1);

			for (UINT32 row = 0; row < reHeight; row++)
			{
				C_UINT32 srcRow = static_cast<UINT32>(row * yBase);

				for (UINT32 col = 0; col < reWidth; col++)
				{
					C_UINT32 srcCol = static_cast<UINT32>(col * xBase);

					purImage.image[row][col] = srcImage.image[srcRow][srcCol];
				}
			}
		}

		void Inner::LinearResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight)
		{
			Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
			Image purImage(pur, reWidth, reHeight, ImageType::GRAY_8BIT);

			C_FLOAT xBase = static_cast<int>(floor(static_cast<float>(width - 1) / static_cast<float>(reWidth - 1) * 1000.0f)) / 1000.0f;
			C_FLOAT yBase = static_cast<int>(floor(static_cast<float>(height - 1) / static_cast<float>(reHeight - 1) * 1000.0f)) / 1000.0f;

			C_FLOAT xProportion = static_cast<float>(reWidth) / static_cast<float>(width);
			C_FLOAT yProportion = static_cast<float>(reHeight) / static_cast<float>(height);
			C_FLOAT xOffset = xProportion - floor(xProportion); // 左邊權重比例
			C_FLOAT yOffset = yProportion - floor(yProportion); // 上邊權重比例

																//(0, 0), (0, 1), (1, 0), (1, 1)
			C_FLOAT w1 = (1.0f - xOffset) * (1.0f - yOffset);
			C_FLOAT w2 = (1.0f - xOffset) * yOffset;
			C_FLOAT w3 = xOffset * (1.0f - yOffset);
			C_FLOAT w4 = xOffset * yOffset;

			for (UINT32 row = 0; row < reHeight; row++)
			{
				float y = row * yBase;
				C_UINT32 y1 = static_cast<UINT32>(floor(y));
				C_UINT32 y2 = static_cast<UINT32>(ceil(y));

				for (UINT32 col = 0; col < reWidth; col++)
				{
					float x = col * xBase;
					C_UINT32 x1 = static_cast<UINT32>(floor(x));
					C_UINT32 x2 = static_cast<UINT32>(ceil(x));
					float pix = static_cast<float>(srcImage.image[y1][x1]) * w1
						+ static_cast<float>(srcImage.image[y1][x2]) * w2
						+ static_cast<float>(srcImage.image[y2][x1]) * w3
						+ static_cast<float>(srcImage.image[y2][x2]) * w4;

					purImage.image[row][col] = static_cast<UINT32>(pix);
				}
			}
		}


		float Inner::BiCubicPloy(float x)
		{
			float absX = abs(x);
			float a = -0.1;

			if (absX <= 1.0)
			{
				return (a + 2) * pow(absX, 3) - (a + 3) * pow(absX, 2) + 1;
			}
			else if (absX < 2.0)
			{
				return a * pow(absX, 3) - 5 * a * pow(absX, 2) + 8 * a * absX - 4 * a;
			}

			return 0.0;
		}

		//blog.csdn.net/linqianbi/article/details/78594019
		void Inner::BiCubicResize8bit(C_UCHAR* src, UCHAR* pur
			, C_UINT32 width, C_UINT32 height
			, C_UINT32 reWidth, C_UINT32 reHeight)
		{
			Image srcImage(const_cast<UCHAR*>(src), width, height, ImageType::GRAY_8BIT);
			Image purImage(pur, reWidth, reHeight, ImageType::GRAY_8BIT);

			float baseWidth = static_cast<float>(reWidth) / static_cast<float>(width);
			float baseHeight = static_cast<float>(reHeight) / static_cast<float>(height);

			for (UINT32 row = 0; row < reWidth; row++)
			{
				for (UINT32 col = 0; col < reHeight; col++)
				{
					//获取目标图像(i,j)在原图中的坐标
					float originalX = static_cast<float>(row) / baseWidth;
					float originalY = static_cast<float>(col) / baseHeight;

					//取出映射到原图的整数部分
					int fixX = static_cast<int>(originalX);
					int fixY = static_cast<int>(originalY);

					//取出映射到原图中的点的四周的16个点的坐标
					int x0 = fixX - 1;
					int y0 = fixY - 1;
					int x1 = fixX;
					int y1 = fixY;
					int x2 = fixX + 1;
					int y2 = fixY + 1;
					int x3 = fixX + 2;
					int y3 = fixY + 2;
					if ((x0 >= 0) && (x3 < width) && (y0 >= 0) && (y3 < height))
					{
						//求出行和列所对应的系数
						float distX0 = BiCubicPloy(originalX - x0);
						float distX1 = BiCubicPloy(originalX - x1);
						float distX2 = BiCubicPloy(originalX - x2);
						float distX3 = BiCubicPloy(originalX - x3);
						float distY0 = BiCubicPloy(originalY - y0);
						float distY1 = BiCubicPloy(originalY - y1);
						float distY2 = BiCubicPloy(originalY - y2);
						float distY3 = BiCubicPloy(originalY - y3);

						//k_i_j=k_i*k_j
						float distX0y0 = distX0 * distY0;
						float distX0y1 = distX0 * distY1;
						float distX0y2 = distX0 * distY2;
						float distX0y3 = distX0 * distY3;
						float distX1y0 = distX1 * distY0;
						float distX1y1 = distX1 * distY1;
						float distX1y2 = distX1 * distY2;
						float distX1y3 = distX1 * distY3;
						float distX2y0 = distX2 * distY0;
						float distX2y1 = distX2 * distY1;
						float distX2y2 = distX2 * distY2;
						float distX2y3 = distX2 * distY3;
						float distX3y0 = distX3 * distY0;
						float distX3y1 = distX3 * distY1;
						float distX3y2 = distX3 * distY2;
						float distX3y3 = distX3 * distY3;

						float pix = (srcImage.image[y0][x0] * distX0y0 +
							srcImage.image[y1][x0] * distX0y1 +
							srcImage.image[y2][x0] * distX0y2 +
							srcImage.image[y3][x0] * distX0y3 +
							srcImage.image[y0][x1] * distX1y0 +
							srcImage.image[y1][x1] * distX1y1 +
							srcImage.image[y2][x1] * distX1y2 +
							srcImage.image[y3][x1] * distX1y3 +
							srcImage.image[y0][x2] * distX2y0 +
							srcImage.image[y1][x2] * distX2y1 +
							srcImage.image[y2][x2] * distX2y2 +
							srcImage.image[y3][x2] * distX2y3 +
							srcImage.image[y0][x3] * distX3y0 +
							srcImage.image[y1][x3] * distX3y1 +
							srcImage.image[y2][x3] * distX3y2 +
							srcImage.image[y3][x3] * distX3y3);

						purImage.image[col][row] = static_cast<UINT32>(pix);
					}
				}
			}
		}


		void PerspectiveTransformNear8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{
			int height = srcImage.Height();
			int width = srcImage.Width();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = minY; col < maxY; col++) {
				int fixY = height - col;
				for (int row = minX; row < maxX; row++) {

					int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
						|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
						continue;
					}

					int tmp = (col - minY) * (maxX - minX) + (row - minX);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					int x = ponits[tmp * 2];
					int y = ponits[tmp * 2 + 1];

					if (x < 0 || x >(width - 1) || y < 0 || y >(height - 1))
						continue;

					purImage.image[col][row] = srcImage.image[y][x];
				}
			}
		}

		void PerspectiveTransformNearReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{
			int height = srcImage.Height();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = fixColMin; col < fixColMax; col++) {

				for (int row = fixRowMin; row < fixRowMax; row++) {

					int tmp = (col - fixColMin) * (fixRowMax - fixRowMin) + (row - fixRowMin);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					int x = ponits[tmp * 2];
					int y = ponits[tmp * 2 + 1];

					//int fixY = height - y;
					//int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					//int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					//int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					//int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					//if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
					//	|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
					//	continue;
					//}

					purImage.image[col][row] = srcImage.image[y][x];
				}
			}
		}


		void PerspectiveTransformLinear8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{

			int height = srcImage.Height();
			int width = srcImage.Width();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = minY; col < maxY; col++) {
				int fixY = height - col;
				for (int row = minX; row < maxX; row++) {


					int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
						|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
						continue;
					}

					int tmp = (col - minY) * (maxX - minX) + (row - minX);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					float x = ponits[tmp * 2];
					float y = ponits[tmp * 2 + 1];

					if (x < 0 || x >(width - 1) || y < 0 || y >(height - 1))
						continue;

					C_FLOAT xOffset = x - floor(x); // 左邊權重比例
					C_FLOAT yOffset = y - floor(y); // 上邊權重比例

					C_FLOAT w1 = (1.0f - xOffset) * (1.0f - yOffset);
					C_FLOAT w2 = (1.0f - xOffset) * yOffset;
					C_FLOAT w3 = xOffset * (1.0f - yOffset);
					C_FLOAT w4 = xOffset * yOffset;

					C_UINT32 linearY1 = static_cast<UINT32>(floor(y));
					C_UINT32 linearY2 = static_cast<UINT32>(ceil(y));

					C_UINT32 linearX1 = static_cast<UINT32>(floor(x));
					C_UINT32 linearX2 = static_cast<UINT32>(ceil(x));

					float pix = static_cast<float>(srcImage.image[linearY1][linearX1]) * w1
						+ static_cast<float>(srcImage.image[linearY1][linearX2]) * w2
						+ static_cast<float>(srcImage.image[linearY2][linearX1]) * w3
						+ static_cast<float>(srcImage.image[linearY2][linearX2]) * w4;

					purImage.image[col][row] = static_cast<UINT32>(pix);
				}
			}
		}

		void PerspectiveTransformLinearReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{
			int height = srcImage.Height();
			int width = srcImage.Width();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = fixColMin; col < fixColMax; col++) {

				for (int row = fixRowMin; row < fixRowMax; row++) {

					int tmp = (col - fixColMin) * (fixRowMax - fixRowMin) + (row - fixRowMin);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					float x = ponits[tmp * 2];
					float y = ponits[tmp * 2 + 1];

					//int fixY = height - y;
					//int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					//int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					//int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					//int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					//if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
					//	|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
					//	continue;
					//}

					if (x < 0 || x > (width - 1) || y < 0 || y >(height - 1))
						continue;

					C_FLOAT xOffset = x - floor(x); // 左邊權重比例
					C_FLOAT yOffset = y - floor(y); // 上邊權重比例

					C_FLOAT w1 = (1.0f - xOffset) * (1.0f - yOffset);
					C_FLOAT w2 = (1.0f - xOffset) * yOffset;
					C_FLOAT w3 = xOffset * (1.0f - yOffset);
					C_FLOAT w4 = xOffset * yOffset;

					C_UINT32 linearY1 = static_cast<UINT32>(floor(y));
					C_UINT32 linearY2 = static_cast<UINT32>(ceil(y));

					C_UINT32 linearX1 = static_cast<UINT32>(floor(x));
					C_UINT32 linearX2 = static_cast<UINT32>(ceil(x));

					float pix = static_cast<float>(srcImage.image[linearY1][linearX1]) * w1
						+ static_cast<float>(srcImage.image[linearY1][linearX2]) * w2
						+ static_cast<float>(srcImage.image[linearY2][linearX1]) * w3
						+ static_cast<float>(srcImage.image[linearY2][linearX2]) * w4;

					purImage.image[col][row] = static_cast<UINT32>(pix);
				}
			}
		}


		void PerspectiveTransformBiCubic8bit(const Image& srcImage, Image& purImage
			, int minX, int minY
			, int maxX, int maxY
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{
			int height = srcImage.Height();
			int width = srcImage.Width();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = minY; col < maxY; col++) {
				int fixY = height - col;
				for (int row = minX; row < maxX; row++) {

					int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
						|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
						continue;
					}

					int tmp = (col - minY) * (maxX - minX) + (row - minX);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					float originalX = ponits[tmp * 2];
					float originalY = ponits[tmp * 2 + 1];

					if (originalX < 0 || originalX >(width - 1) || originalY < 0 || originalY > (height - 1))
						continue;



					int fixX = static_cast<int>(originalX);
					int fixY = static_cast<int>(originalY);

					//取出映射到原图中的点的四周的16个点的坐标
					int biCubicX0 = fixX - 1;
					int biCubicY0 = fixY - 1;
					int biCubicX1 = fixX;
					int biCubicY1 = fixY;
					int biCubicX2 = fixX + 1;
					int biCubicY2 = fixY + 1;
					int biCubicX3 = fixX + 2;
					int biCubicY3 = fixY + 2;
					if ((biCubicX0 >= 0) && (biCubicX3 < width) && (biCubicY0 >= 0) && (biCubicY3 < height))
					{
						float distX0 = BiCubicPloy(originalX - biCubicX0);
						float distX1 = BiCubicPloy(originalX - biCubicX1);
						float distX2 = BiCubicPloy(originalX - biCubicX2);
						float distX3 = BiCubicPloy(originalX - biCubicX3);
						float distY0 = BiCubicPloy(originalY - biCubicY0);
						float distY1 = BiCubicPloy(originalY - biCubicY1);
						float distY2 = BiCubicPloy(originalY - biCubicY2);
						float distY3 = BiCubicPloy(originalY - biCubicY3);

						float distX0y0 = distX0 * distY0;
						float distX0y1 = distX0 * distY1;
						float distX0y2 = distX0 * distY2;
						float distX0y3 = distX0 * distY3;
						float distX1y0 = distX1 * distY0;
						float distX1y1 = distX1 * distY1;
						float distX1y2 = distX1 * distY2;
						float distX1y3 = distX1 * distY3;
						float distX2y0 = distX2 * distY0;
						float distX2y1 = distX2 * distY1;
						float distX2y2 = distX2 * distY2;
						float distX2y3 = distX2 * distY3;
						float distX3y0 = distX3 * distY0;
						float distX3y1 = distX3 * distY1;
						float distX3y2 = distX3 * distY2;
						float distX3y3 = distX3 * distY3;

						float pix = (srcImage.image[biCubicY0][biCubicX0] * distX0y0 +
							srcImage.image[biCubicY1][biCubicX0] * distX0y1 +
							srcImage.image[biCubicY2][biCubicX0] * distX0y2 +
							srcImage.image[biCubicY3][biCubicX0] * distX0y3 +
							srcImage.image[biCubicY0][biCubicX1] * distX1y0 +
							srcImage.image[biCubicY1][biCubicX1] * distX1y1 +
							srcImage.image[biCubicY2][biCubicX1] * distX1y2 +
							srcImage.image[biCubicY3][biCubicX1] * distX1y3 +
							srcImage.image[biCubicY0][biCubicX2] * distX2y0 +
							srcImage.image[biCubicY1][biCubicX2] * distX2y1 +
							srcImage.image[biCubicY2][biCubicX2] * distX2y2 +
							srcImage.image[biCubicY3][biCubicX2] * distX2y3 +
							srcImage.image[biCubicY0][biCubicX3] * distX3y0 +
							srcImage.image[biCubicY1][biCubicX3] * distX3y1 +
							srcImage.image[biCubicY2][biCubicX3] * distX3y2 +
							srcImage.image[biCubicY3][biCubicX3] * distX3y3);

						purImage.image[col][row] = static_cast<UINT32>(pix);
					}
					else
					{
						purImage.image[col][row] = srcImage.image[fixY][fixX];
					}
				}
			}
		}

		void PerspectiveTransformBiCubicReverse8bit(const Image& srcImage, Image& purImage
			, int fixRowMin, int fixRowMax
			, int fixColMin, int fixColMax
			, int x0, int y0
			, int x1, int y1
			, int x2, int y2
			, int x3, int y3
			, const vector<float>& ponits)
		{
			int height = srcImage.Height();
			int width = srcImage.Width();
			int fixY0 = height - y0;
			int fixY1 = height - y1;
			int fixY2 = height - y2;
			int fixY3 = height - y3;

			for (int col = fixColMin; col < fixColMax; col++) {

				for (int row = fixRowMin; row < fixRowMax; row++) {

					int tmp = (col - fixColMin) * (fixRowMax - fixRowMin) + (row - fixRowMin);
					//if (tmp < 0 || ponits.size() - 1 < (tmp << 1) + 1) continue;
					//int x = ponits[tmp * 2];
					//int y = ponits[tmp * 2 + 1];

					//int fixY = height - y;
					//int line1 = (x1 - x0) * (fixY - fixY0) - (fixY1 - fixY0) * (row - x0);
					//int line2 = (x3 - x1) * (fixY - fixY1) - (fixY3 - fixY1) * (row - x1);
					//int line3 = (x2 - x3) * (fixY - fixY3) - (fixY2 - fixY3) * (row - x3);
					//int line4 = (x0 - x2) * (fixY - fixY2) - (fixY0 - fixY2) * (row - x2);

					//if (!((line1 >= 0 && line2 >= 0 && line3 >= 0 && line4 >= 0)
					//	|| (line1 <= 0 && line2 <= 0 && line3 <= 0 && line4 <= 0))) {
					//	continue;
					//}

					float originalX = ponits[tmp * 2];
					float originalY = ponits[tmp * 2 + 1];



					int fixX = static_cast<int>(originalX);
					int fixY = static_cast<int>(originalY);

					//取出映射到原图中的点的四周的16个点的坐标
					int biCubicX0 = fixX - 1;
					int biCubicY0 = fixY - 1;
					int biCubicX1 = fixX;
					int biCubicY1 = fixY;
					int biCubicX2 = fixX + 1;
					int biCubicY2 = fixY + 1;
					int biCubicX3 = fixX + 2;
					int biCubicY3 = fixY + 2;
					if ((biCubicX0 >= 0) && (biCubicX3 < width) && (biCubicY0 >= 0) && (biCubicY3 < height))
					{
						float distX0 = BiCubicPloy(originalX - biCubicX0);
						float distX1 = BiCubicPloy(originalX - biCubicX1);
						float distX2 = BiCubicPloy(originalX - biCubicX2);
						float distX3 = BiCubicPloy(originalX - biCubicX3);
						float distY0 = BiCubicPloy(originalY - biCubicY0);
						float distY1 = BiCubicPloy(originalY - biCubicY1);
						float distY2 = BiCubicPloy(originalY - biCubicY2);
						float distY3 = BiCubicPloy(originalY - biCubicY3);

						float distX0y0 = distX0 * distY0;
						float distX0y1 = distX0 * distY1;
						float distX0y2 = distX0 * distY2;
						float distX0y3 = distX0 * distY3;
						float distX1y0 = distX1 * distY0;
						float distX1y1 = distX1 * distY1;
						float distX1y2 = distX1 * distY2;
						float distX1y3 = distX1 * distY3;
						float distX2y0 = distX2 * distY0;
						float distX2y1 = distX2 * distY1;
						float distX2y2 = distX2 * distY2;
						float distX2y3 = distX2 * distY3;
						float distX3y0 = distX3 * distY0;
						float distX3y1 = distX3 * distY1;
						float distX3y2 = distX3 * distY2;
						float distX3y3 = distX3 * distY3;

						float pix = (srcImage.image[biCubicY0][biCubicX0] * distX0y0 +
							srcImage.image[biCubicY1][biCubicX0] * distX0y1 +
							srcImage.image[biCubicY2][biCubicX0] * distX0y2 +
							srcImage.image[biCubicY3][biCubicX0] * distX0y3 +
							srcImage.image[biCubicY0][biCubicX1] * distX1y0 +
							srcImage.image[biCubicY1][biCubicX1] * distX1y1 +
							srcImage.image[biCubicY2][biCubicX1] * distX1y2 +
							srcImage.image[biCubicY3][biCubicX1] * distX1y3 +
							srcImage.image[biCubicY0][biCubicX2] * distX2y0 +
							srcImage.image[biCubicY1][biCubicX2] * distX2y1 +
							srcImage.image[biCubicY2][biCubicX2] * distX2y2 +
							srcImage.image[biCubicY3][biCubicX2] * distX2y3 +
							srcImage.image[biCubicY0][biCubicX3] * distX3y0 +
							srcImage.image[biCubicY1][biCubicX3] * distX3y1 +
							srcImage.image[biCubicY2][biCubicX3] * distX3y2 +
							srcImage.image[biCubicY3][biCubicX3] * distX3y3);

						purImage.image[col][row] = static_cast<UINT32>(pix);
					}
					else
					{
						purImage.image[col][row] = srcImage.image[fixY][fixX];
					}
				}
			}
		}

	}
}