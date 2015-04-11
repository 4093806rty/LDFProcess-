/**********************************************************
 * ��ͼ�����ࡣ
 *
 *
 *********************************************************/

#pragma once

#include "Core.h"

namespace UltraGIS
{
	class GMap
	{
	public:
		GMap(const CSize & viewSize, const Rectangle2D & bounds) throw(...);
		~GMap(void);

		// ����ת�����ܷ�װ
		void GeoToMap(double geoX, double geoY, POINT * mapPoint);
		void MapToGeo(int mapX, int mapY, Point2D * geoPoint);

	private:
		CSize m_size;			// ���ڴ�С
		Point2D m_origin;		// �Ӵ�ԭ�㣨�Ӵ����Ͻǵľ�γ�ȣ�
		double m_ratio;			// ����ϵ��
	};
}