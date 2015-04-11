/******************************************************************************
 * 名称：几何绘图类。
 * 设计思路：用于将Geometry对象绘制到屏幕上，将投影变换和坐标转换工作封装在GDC内部。
 *
 * 
 * 
 * 
 *****************************************************************************/

#pragma once

#include "Core.h"

namespace UltraGIS
{

	class GDC
	{
	public:
		GDC(CDC * pDC, const Rectangle2D & bounds);
		~GDC(void);
	
	public:
		// 绘图操作系列函数
		Point2D GetCurrentPosition() const;
		void MoveTo(double x, double y);
		void MoveTo(const Point2D & pt);
		bool LineTo(double x, double y);
		bool LineTo(const Point2D & pt);

	private:
		// 坐标转换功能封装
		void GeoToView(double x, double y, POINT * mapPoint);
		void ViewToGeo(int x, int y, Point2D * geoPoint);

	private:
		CDC * m_pDC;			// 窗口绘图类
		Rectangle2D m_bounds;	// 地图边界
		CSize m_size;			// 窗口大小
		double m_ratio;			// 缩放系数
	};
}