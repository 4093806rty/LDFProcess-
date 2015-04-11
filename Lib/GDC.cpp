
#include "StdAfx.h"
#include "GDC.h"

namespace UltraGIS
{

	GDC::GDC(CDC * pDC, const Rectangle2D & bounds)
	{
		ASSERT_VALID(pDC);

		m_pDC = pDC;
		m_bounds = bounds;

		// 计算缩放系数以便于快速运算
		CRect rect;
		pDC->GetWindow()->GetClientRect(&rect);
		m_size = rect.Size();
		m_ratio = min( m_size.cx / bounds.getWidth(), m_size.cy / bounds.getHeight() );
 	}

	GDC::~GDC(void)
	{
	}

	void GDC::GeoToView(double geoX, double geoY, POINT * pt)
	{
		pt->x = (int)((geoX - m_bounds.Xmin) * m_ratio);
		pt->y = (int)(m_size.cy - (geoY - m_bounds.Ymin) * m_ratio );
	}

	void GDC::ViewToGeo(int x, int y, Point2D * geoPoint)
	{
	}

	void GDC::MoveTo(double x, double y)
	{
		POINT pt;
		GeoToView(x, y, &pt);
		m_pDC->MoveTo(pt);
	}

	void GDC::MoveTo(const Point2D & geoPoint)
	{
		POINT pt;
		GeoToView(geoPoint.X, geoPoint.Y, &pt);
		m_pDC->MoveTo(pt);
	}

	bool GDC::LineTo(double x, double y)
	{
		POINT pt;
		GeoToView(x, y, &pt);
		return m_pDC->LineTo(pt);
	}

	bool GDC::LineTo(const Point2D & geoPoint)
	{
		POINT pt;
		GeoToView(geoPoint.X, geoPoint.Y, &pt);
		return m_pDC->LineTo(pt);
	}
}