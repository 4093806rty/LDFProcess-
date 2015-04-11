/************************************************************************
	�ļ���: Geometry.h
	������: ���
	����: 
		�ο�OpenGIS�淶��������ļ��ζ������͡�

	��ʷ��¼:	
		2008��1��9����괴��������GeometryType�ࡢGeometry��GeoPoint�ࡣ

************************************************************************/

#pragma once

#include "Core.h"

namespace UltraGIS 
{

	// �����ļ�������
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
	
	// �������͵Ļ���
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


	// ��
	class GeoPoint
	{
	public:
		GeoPoint(void) throw();
		GeoPoint(double x, double y) throw();
		GeoPoint(const Point2D & pt) throw();

		const Point2D& getValue(void) const throw();

		// ˵����
		// ��(Point2D)��ת��ʱ�Ի������ʱ����
		// ��(const Point2D&)��ʽ�ĺ������ݲ��������ʱ����
		//operator const Point2D&() const throw() { return m_pt; }

	private:
		Point2D m_pt;
	};

	
	// ��
	class GeoLineString
	{
	public:
		GeoLineString() throw(...);
		GeoLineString(Point2D * pts, int numPoints) throw(...);
		virtual ~GeoLineString(void) throw(...);

		int getCount(void) const throw();
		const Point2D& getAt(int idx) const throw(...);
		bool IsClosed(void) const throw(...);		//�Ƿ���

	private:
		Point2D * m_pts;
		int m_count;
	};


	// ��
	class GeoLinearRing : GeoLineString
	{
	public:
		GeoLinearRing() throw(...);
		GeoLinearRing(Point2D * pts, int numPoints) throw(...);
	};


	// ��
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
