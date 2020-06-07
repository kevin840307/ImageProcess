#pragma once
#ifndef DRAW_H
#define DRAW_H
#include "general.h"
#include "Image.h"
#include "Point.h"

namespace IVC {

	void DrawLine8bit(Image& image, const Point& p1, const Point& p2, C_UCHAR& color = 255);

	void DrawPoint8bit(Image& image, const Point& point, C_UCHAR& color = 255);

}

#endif // !DRAW_H