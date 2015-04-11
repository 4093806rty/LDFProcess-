/**********************************************************
 * 地图窗口类。
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

		// 坐标转换功能封装
		void GeoToMap(double geoX, double geoY, POINT * mapPoint);
		void MapToGeo(int mapX, int mapY, Point2D * geoPoint);

	private:
		CSize m_size;			// 窗口大小
		Point2D m_origin;		// 视窗原点（视窗左上角的经纬度）
		double m_ratio;			// 缩放系数
	};
}