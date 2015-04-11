/******************************************************************************
 * ���ƣ����λ�ͼ�ࡣ
 * ���˼·�����ڽ�Geometry������Ƶ���Ļ�ϣ���ͶӰ�任������ת��������װ��GDC�ڲ���
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
		// ��ͼ����ϵ�к���
		Point2D GetCurrentPosition() const;
		void MoveTo(double x, double y);
		void MoveTo(const Point2D & pt);
		bool LineTo(double x, double y);
		bool LineTo(const Point2D & pt);

	private:
		// ����ת�����ܷ�װ
		void GeoToView(double x, double y, POINT * mapPoint);
		void ViewToGeo(int x, int y, Point2D * geoPoint);

	private:
		CDC * m_pDC;			// ���ڻ�ͼ��
		Rectangle2D m_bounds;	// ��ͼ�߽�
		CSize m_size;			// ���ڴ�С
		double m_ratio;			// ����ϵ��
	};
}