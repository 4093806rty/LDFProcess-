#include "StdAfx.h"
#include "ShapeProvider.h"

#include <assert.h>

namespace UltraGIS
{

	ShapeProvider::ShapeProvider(LPCTSTR lpszPathName)
	{
		m_pShpData = NULL;
		m_pathName = std::wstring(lpszPathName);		
	}

	ShapeProvider::~ShapeProvider(void)
	{
		if ( m_pShpData )
			delete[] m_pShpData;
	}

	bool ShapeProvider::LoadShp(LPCTSTR lpszPathName)
	{
		assert( m_pShpData == NULL );
		assert( m_vecPShp.empty() );

		FILE * file = _tfopen( lpszPathName, _T("rb") );
		if ( ! file )
			return false;

		// read header
		::fread( &m_header, 1, sizeof(m_header), file );

		// load data
		int dataLength = m_header.FileLength() * 2 - sizeof(m_header);
		byte * buffer = new byte[dataLength];
		memset( buffer, 0, dataLength );
		::fread( buffer, 1, dataLength, file );
		this->m_pShpData = buffer;

		// parse geometries
		shp::ShapeObject * pObj = (shp::ShapeObject *)(buffer);
		while ( (byte *)pObj - buffer < dataLength)
		{
			m_vecPShp.push_back( pObj );
			pObj = (shp::ShapeObject *)((BYTE *)pObj + 8 + pObj->Length() * 2);
		}

		::fclose( file );
		return true;
	}

	std::vector<const shp::ShapeObject *> ShapeProvider::getGeometries(void)
	{
		return this->m_vecPShp;
	}

	Rectangle2D ShapeProvider::getExtents(void)
	{
		Rectangle2D bounds;
		bounds.Xmin = m_header.Xmin;
		bounds.Ymin = m_header.Ymin;
		bounds.Xmax = m_header.Xmax;
		bounds.Ymax = m_header.Ymax;
		return bounds;
	}
	
	bool ShapeProvider::Open(void)
	{
		this->Close();
		return this->LoadShp( this->m_pathName.c_str() );
	}

	void ShapeProvider::Close(void)
	{
		if ( m_pShpData )
		{
			this->m_vecPShp.clear();
			memset( &m_header, 0, sizeof(m_header) );

			delete[] m_pShpData;
			m_pShpData = NULL;
		}
	}
}