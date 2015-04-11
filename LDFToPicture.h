#pragma once
#include "./Lib/LDF.h"
#include "./lib/IProvider.h"
#include "./Lib/Core.h"

using namespace UltraGIS;

class LDFToPicture
{
public:
	LDFToPicture();
	~LDFToPicture();
	int setLDF(LDF& ldf);
	int setTIFF(LDF& ldf,float *KValue);
	//int setMEAN(LDF& ldf,float *Mean);
	int Render(CDC* pDC, CRect rc = CRect(0, 0, 0, 0));

	int Renderlegend(CDC* pDC, CRect rc = CRect(0, 0, 0, 0));

	void DrawMap_arg(IProvider * pProvider,Gdiplus::Color Color_arg,int pen_width,int exception);
	void GeoToMap(const shp::Point * geoPts, POINT * pts, int count);
	void GeoToMap(double geoX, double geoY, POINT * pt);
	void MapToGeo(int x, int y, Point2D * geoPoint);

	void Free();

	Gdiplus::Bitmap* m_pImage; 
	CSize GetImageSize() const;

	int LDF2Bmp(LDF& ldf);
	int calColor();
	void setRange(int Channel,double min, double max,int fontsize);
	void setColor(std::vector<Gdiplus::Color>& colorTab);
	int Save(CString filepath);
	BOOL createImage();//LDF& ldf);

	Gdiplus::Color strechColor(float val);
	int drawedge(LDF& ldf);
	int LDFToPicture::draw_StatisticArea(LDF& ldf,IProvider * pProvider,int style,COLORREF col,float Lon,float Lat,float Width,float Height);
	double m_dmin;
	double m_dmax;
	double m_dMinLon;
	double m_dMaxLon;
	double m_dMinLat;
	double m_dMaxLat;
	double m_dratioX;
	double m_dratioY;
	Point2D m_origin;		// 视窗原点（视窗左上角的经纬度）

	int m_Width_top;//图例：处边框宽度（右、上、无经纬度处及下）//绘图时，在其他类里会依据字体修改白色条型外边框的宽度
	int m_Width_bottom;
	int m_Width_left;//图例：左侧外边界宽度（因有经纬度）//绘图时，在其他类里会依据字体修改白色条型外边框的宽度
	int m_Width_right;
	int m_fontsize;//图例：经纬度字体大小
	int m_step;//图例：经纬度间隔
	int m_fontsize_palette;//调色板图例：字体大小
	int m_nSample;
	int m_nLine;
	int m_Channel;//待计算通道
	float * m_pData;
	std::vector<Gdiplus::Color> m_aColorTab;
};

