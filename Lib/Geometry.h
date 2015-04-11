/************************************************************************
	文件名: Geometry.h
	创建者: 李宏
	描述: 
		参考OpenGIS规范定义基本的几何对象类型。

	历史记录:	
		2008年1月9日李宏创建。定义GeometryType类、Geometry、GeoPoint类。

************************************************************************/

#pragma once

#include "Core.h"

namespace UltraGIS 
{

	// 基本的几何类型
	enum GeometryType
	{
		Point,
		LineString,
		Polygon,
		//MultiPoint,
		//MultiLineString,
		//MultiPolygon,
		//GeometryCollection,
	};
	
	// 几何类型的基类
	class Geometry
	{
	protected:
		Geometry(void) throw();

	public:
		GeometryType getType(void) const throw();
		const Rectangle2D& getMBR(void) const throw();

	protected:
		GeometryType m_gtype;
		Rectangle2D m_mbr;
	};


	// 点
	class GeoPoint
	{
	public:
		GeoPoint(void) throw();
		GeoPoint(double x, double y) throw();
		GeoPoint(const Point2D & pt) throw();

		const Point2D& getValue(void) const throw();

		// 说明：
		// 用(Point2D)做转换时仍会产生临时对象。
		// 给(const Point2D&)形式的函数传递不会产生临时对象。
		//operator const Point2D&() const throw() { return m_pt; }

	private:
		Point2D m_pt;
	};

	
	// 线
	class GeoLineString
	{
	public:
		GeoLineString() throw(...);
		GeoLineString(Point2D * pts, int numPoints) throw(...);
		virtual ~GeoLineString(void) throw(...);

		int getCount(void) const throw();
		const Point2D& getAt(int idx) const throw(...);
		bool IsClosed(void) const throw(...);		//是否封闭

	private:
		Point2D * m_pts;
		int m_count;
	};


	// 环
	class GeoLinearRing : GeoLineString
	{
	public:
		GeoLinearRing() throw(...);
		GeoLinearRing(Point2D * pts, int numPoints) throw(...);
	};


	// 面
	class GeoPolygon
	{
	public:
		GeoPolygon(void) throw(...);
		GeoPolygon(Point2D * pts, int numPoints) throw(...);
		GeoPolygon(GeoLinearRing * rings, int numRings) throw(...);

		int getCount(void) const throw();
		const GeoLinearRing& getAt(int idx) const throw(...);

	private:
		GeoLinearRing * m_rings;
		int m_count;
	};
}
