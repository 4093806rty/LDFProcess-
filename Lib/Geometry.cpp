#include "StdAfx.h"
#include "Core.h"
#include "Geometry.h"

namespace UltraGIS
{
	#pragma region Geometry

	Geometry::Geometry()
	{
	}

	GeometryType Geometry::getType() const
	{
		return m_gtype;
	}

	const Rectangle2D& Geometry::getMBR() const
	{
		return m_mbr;
	}

	#pragma endregion

	#pragma region GeoPoint

	GeoPoint::GeoPoint() : m_pt()
	{
	}

	GeoPoint::GeoPoint(double x, double y) : m_pt(x, y)
	{
	}

	GeoPoint::GeoPoint(const Point2D &pt) : m_pt(pt)
	{
	}

	const Point2D& GeoPoint::getValue(void) const
	{
		return m_pt;
	}

	#pragma endregion 

	#pragma region GeoLineString

	GeoLineString::GeoLineString() : m_pts(NULL), m_count(0)
	{
	}

	GeoLineString::GeoLineString(Point2D * pts, int numPoints)
	{
		//if ( numPoints < 2 )
		//	::AfxThrowInvalidArgException();

		m_pts = new Point2D[numPoints];
		m_count = numPoints;
	}

	GeoLineString::~GeoLineString(void)
	{
		delete[] m_pts;
		m_count = 0;
	}

	int GeoLineString::getCount(void) const
	{
		return m_count;
	}

	const Point2D& GeoLineString::getAt(int idx) const
	{
		return m_pts[idx];
	}

	bool GeoLineString::IsClosed(void) const
	{
		return m_pts[0] == m_pts[m_count-1];
	}

	#pragma endregion

	#pragma region GeoLinearRing

	GeoLinearRing::GeoLinearRing() : GeoLineString()
	{
	}

	GeoLinearRing::GeoLinearRing(Point2D * pts, int numPoints) : GeoLineString(pts, numPoints)
	{
		//if ( numPoints < 3 || pts[0] != pts[numPoints-1] )
		//	::AfxThrowInvalidArgException();
	}

	#pragma endregion

	#pragma region GeoPolygon

	GeoPolygon::GeoPolygon() : m_rings(NULL), m_count(0)
	{
	}

	GeoPolygon::GeoPolygon(Point2D * pts, int numPoints)
	{
		m_rings = new GeoLinearRing(pts, numPoints);
		m_count = 1;
	}

	GeoPolygon::GeoPolygon(GeoLinearRing * rings, int numRings)
	{
		m_rings = new GeoLinearRing[numRings];
	}

	int GeoPolygon::getCount() const
	{
		return m_count;
	}
	
	const GeoLinearRing& GeoPolygon::getAt(int idx) const
	{
		return m_rings[idx];
	}

	#pragma endregion
}
