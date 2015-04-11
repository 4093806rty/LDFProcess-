#include "StdAfx.h"
#include "GMap.h"

namespace UltraGIS
{

	GMap::GMap(const CSize & viewSize, const Rectangle2D & bounds)
	{
		ASSERT( ! bounds.IsEmpty() );

		m_size = viewSize;
		m_origin = Point2D(bounds.Xmin, bounds.Ymax);
		m_ratio = min( m_size.cx / bounds.getWidth(), m_size.cy / bounds.getHeight() );
	}

	GMap::~GMap(void)
	{
	}

	void GMap::GeoToMap(double geoX, double geoY, POINT * pt)
	{
		pt->x = (int)((geoX - m_origin.X) * m_ratio);
		pt->y = (int)((m_origin.Y - geoY) * m_ratio);
	}

	void GMap::MapToGeo(int x, int y, Point2D * geoPoint)
	{
		geoPoint->X = (x / m_ratio) + m_origin.X;
		geoPoint->Y = m_origin.Y - (y / m_ratio);
	}
}