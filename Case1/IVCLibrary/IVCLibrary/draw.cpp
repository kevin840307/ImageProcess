#include "draw.h"
namespace IVC {

	void DrawLine8bit(Image& image, const Point& p1, const Point& p2, C_UCHAR& color)
	{
		assert(image.Width() > p1.X() && image.Width() > p2.X());
		assert(image.Height() > p1.Y() && image.Height() > p2.Y());

		C_UINT32 absDiffX = abs(static_cast<int32_t>(p1.X()) - static_cast<int32_t>(p2.X()));
		C_UINT32 absDiffY = abs(static_cast<int32_t>(p1.Y()) - static_cast<int32_t>(p2.Y()));
		C_UINT32 base = absDiffX > absDiffY ? absDiffX : absDiffY;
		C_INT32 diffX = static_cast<int32_t>(p1.X()) - static_cast<int32_t>(p2.X());
		C_INT32 diffY = static_cast<int32_t>(p1.Y()) - static_cast<int32_t>(p2.Y());
		C_FLOAT baseX = -static_cast<float>(diffX) / static_cast<float>(base);
		C_FLOAT baseY = -static_cast<float>(diffY) / static_cast<float>(base);

		float x = static_cast<float>(p1.X());
		float y = static_cast<float>(p1.Y());

		for (UINT32 index = 0; index < base; index++)
		{
			image.image[static_cast<UINT32>(y)][static_cast<UINT32>(x)] = color;
			x += baseX;
			y += baseY;
		}
	}

	void DrawPoint8bit(Image& image, const Point& point, C_UCHAR& color)
	{
		assert(image.Width() > point.X() && image.Height() > point.Y());

		image.image[point.Y()][point.X()] = color;
	}
}