/******************************************************************************
 * 基础数据类型
 *
 *
 *****************************************************************************/

#pragma once

namespace UltraGIS
{
	struct Point2D
	{
	public:
		Point2D(void) throw();
		Point2D(double x, double y) throw();

		bool operator==(const Point2D& pt) throw();
		bool operator!=(const Point2D& pt) throw();

	public:
		double X;
		double Y;
	};
	
	struct Rectangle2D
	{
	public:
		Rectangle2D(void) throw();
		Rectangle2D(double x, double y, double width, double height) throw();
		Rectangle2D(const Point2D& p1, const Point2D& p2) throw();

	public:
		double getWidth(void) const throw();
		double getHeight(void) const throw();
		bool IsEmpty(void) const throw();
		bool IsNormalize(void) const throw();
		void Normalize() throw();

	public:
		double Xmin;
        double Ymin;
        double Xmax;
        double Ymax;
	};
}