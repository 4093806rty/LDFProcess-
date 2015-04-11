/***************************************************
 * 空间数据提供者。
 * 
 ***************************************************/

#pragma once

#include "Core.h"
#include "ShapeFile.h"
#include <vector>

namespace UltraGIS 
{

	class IProvider
	{
	public:
		virtual ~IProvider() {};
		
	public:
		typedef std::vector<const shp::ShapeObject *>::const_iterator ITER_SHP;

	public:
		virtual bool Open(void) = 0;
		virtual void Close(void) = 0;
		virtual std::vector<const shp::ShapeObject *> getGeometries(void) = 0;
		virtual Rectangle2D getExtents(void) = 0;
	};
}
