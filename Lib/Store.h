/******************************************************************************
 * 几何数据的存储
 *
 *****************************************************************************/

#pragma once

#pragma warning(push)
#pragma warning(disable:4200)

#pragma pack(push)
#pragma pack(1)

namespace UltraGIS { namespace store {

	typedef int Integer;
	typedef int Coord;

	struct SPoint
	{
		Coord x;
		Coord y;
	};

	struct SRect
	{
		Coord xmin;
		Coord ymin;
		Coord xmax;
		Coord ymax;
	};

	struct GObject
	{
		Integer GeoType;
		SRect MBR;
	};

	struct GPoint : public GObject
	{
		Coord X;
		Coord Y;
	};

	struct GLineString : public GObject
	{
		Integer NumPoints;
		SPoint Points[];		// NumPoints
	};

	struct GPolygon : public GObject
	{
		Integer NumParts;
		Integer NumPoints;
		Integer Parts[];		// NumParts
		// SPoint Points[];		// NumPoints

		// 获取坐标点
		inline const SPoint& Points(Integer pos) const 
		{
			return *(SPoint *)((byte *)this 
				+ sizeof(GPolygon) 
				+ sizeof(Integer) * NumParts 
				+ sizeof(SPoint) * pos);
		}
	};

}}

#pragma pack(pop)

#pragma warning(pop)