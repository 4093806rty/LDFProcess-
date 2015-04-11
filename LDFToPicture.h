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
	Point2D m_origin;		// �Ӵ�ԭ�㣨�Ӵ����Ͻǵľ�γ�ȣ�

	int m_Width_top;//ͼ�������߿��ȣ��ҡ��ϡ��޾�γ�ȴ����£�//��ͼʱ����������������������޸İ�ɫ������߿�Ŀ��
	int m_Width_bottom;
	int m_Width_left;//ͼ���������߽��ȣ����о�γ�ȣ�//��ͼʱ����������������������޸İ�ɫ������߿�Ŀ��
	int m_Width_right;
	int m_fontsize;//ͼ������γ�������С
	int m_step;//ͼ������γ�ȼ��
	int m_fontsize_palette;//��ɫ��ͼ���������С
	int m_nSample;
	int m_nLine;
	int m_Channel;//������ͨ��
	float * m_pData;
	std::vector<Gdiplus::Color> m_aColorTab;
};

