#include "StdAfx.h"
#include "math.h"
#include "Core.h"

namespace UltraGIS 
{
	#pragma region Point2D

	Point2D::Point2D(void) : X(0), Y(0)
	{
	}

	Point2D::Point2D(double x, double y) : X(x), Y(y)
	{
	}

	bool Point2D::operator==(const Point2D& pt)
	{
		return X == pt.X && Y == pt.Y;
	}

	bool Point2D::operator!=(const Point2D& pt)
	{
		return X != pt.X || Y != pt.Y;
	}

	#pragma endregion

	#pragma region Rectangle2D

	Rectangle2D::Rectangle2D(void) : Xmin(0), Ymin(0), Xmax(0), Ymax(0)
	{
	}

	Rectangle2D::Rectangle2D(double x, double y, double width, double height)
	{
		Xmin = min(x, x + width);
		Ymin = min(y, y + height);
		Xmax = max(x, x + width);
		Ymax = max(y, y + height);
	}

	Rectangle2D::Rectangle2D(const Point2D& p1, const Point2D& p2)
	{
		Xmin = min(p1.X, p2.X);
		Ymin = min(p1.Y, p2.Y);
		Xmax = max(p1.X, p2.X);
		Ymax = max(p1.Y, p2.Y);
	}

	double Rectangle2D::getWidth(void) const
	{
		return abs(Xmax - Xmin);
	}

	double Rectangle2D::getHeight(void) const
	{
		return abs(Ymax - Ymin);
	}

	bool Rectangle2D::IsEmpty(void) const
	{
		return (Xmin == Xmax && Ymin == Ymax);
	}

	bool Rectangle2D::IsNormalize(void) const
	{
		return (Xmax >= Xmin && Ymax >= Ymin);
	}

	void Rectangle2D::Normalize() throw()
	{
		Xmin = min(Xmin, Xmax);
		Ymin = min(Ymin, Ymax);
		Xmax = max(Xmin, Xmax);
		Ymax = max(Ymin, Ymax);
	}

	#pragma endregion
}