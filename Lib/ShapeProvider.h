#pragma once

#include "ShapeFile.h"
#include "IProvider.h"
#include <vector>
#include <string>

namespace UltraGIS
{
	class ShapeProvider : public IProvider
	{
	public:
		ShapeProvider(LPCTSTR lpszPathName);
		~ShapeProvider(void);

	private:
		// ±‹√‚«≥øΩ±¥
		ShapeProvider(const ShapeProvider & provider) {};
		
	public:
		virtual bool Open(void);
		virtual void Close(void);
		virtual std::vector<const shp::ShapeObject *> getGeometries(void);
		virtual Rectangle2D getExtents(void);

	private:
		bool LoadShx(LPCTSTR lpszPathName);
		bool LoadShp(LPCTSTR lpszPathName);

	private:
		std::wstring m_pathName;
		shp::ShapeHeader m_header;
		byte * m_pShpData;
		std::vector<const shp::ShapeObject *> m_vecPShp;
	};
}
