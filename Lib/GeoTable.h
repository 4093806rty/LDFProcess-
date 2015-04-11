/**********************************************************
 * �������ζ�������ݱ��ࡣ
 *
 *
 *********************************************************/

#pragma once

#include "Core.h"
#include "Geometry.h"
#include <vector>


namespace UltraGIS
{

	class GeoTable
	{
	public:
		GeoTable(void);
		~GeoTable(void);

	private:
		std::vector<Geometry> m_vecGeom;
	};

}