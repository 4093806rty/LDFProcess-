#include "stdafx.h"
#include "LDFToPicture.h"

ULONG_PTR m_gdiplusToken;
LDFToPicture::LDFToPicture()
{
	m_dmin = 2450;//ħ������NO/�ѱ�CLDFProcessDlg����
	m_dmax = 2910;
	//��γ���ֺŵȲ���˵����ͷ�ļ�
	m_fontsize=50;//����NO/�ѱ�����
	m_fontsize_palette=8;//����NO/�ѱ�����
	m_step=5;
	m_Channel=5;//Ĭ�ϵ�5����
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
	//������
	m_aColorTab.push_back(Gdiplus::Color(0, 0, 139));//00008B
	m_aColorTab.push_back(Gdiplus::Color(0, 0, 255));//0000FF
	m_aColorTab.push_back(Gdiplus::Color(0, 200, 255));//FFC800//���Ĵ�
	m_aColorTab.push_back(Gdiplus::Color(0, 139, 0));//008B00
	m_aColorTab.push_back(Gdiplus::Color(128, 255, 128));//80FF80
	m_aColorTab.push_back(Gdiplus::Color(255, 255, 0));//FFFF00
	m_aColorTab.push_back(Gdiplus::Color(255, 150, 0));//FF9600
	m_aColorTab.push_back(Gdiplus::Color(255, 0, 0));//FF0000
	m_aColorTab.push_back(Gdiplus::Color(150, 0, 0));//960000
	m_aColorTab.push_back(Gdiplus::Color(255, 0, 255));//FF00FF


/*	//������
	m_aColorTab.push_back(Gdiplus::Color(0, 0, 139));//00008B
	m_aColorTab.push_back(Gdiplus::Color(0, 0, 255));//0000FF
	//m_aColorTab.push_back(Gdiplus::Color(0, 0, 200));//00C8FF
	m_aColorTab.push_back(Gdiplus::Color(0, 139, 0));//008B00
	m_aColorTab.push_back(Gdiplus::Color(128, 255, 128));//80FF80
	m_aColorTab.push_back(Gdiplus::Color(255, 255, 0));//FFFF00
	m_aColorTab.push_back(Gdiplus::Color(255, 150, 0));//FF9600
	m_aColorTab.push_back(Gdiplus::Color(255, 0, 0));//FF0000
	m_aColorTab.push_back(Gdiplus::Color(150, 0, 0));//960000
	m_aColorTab.push_back(Gdiplus::Color(255, 0, 255));//FF00FF*/

/*	//����һ
	m_aColorTab.push_back(Gdiplus::Color::Blue);
	m_aColorTab.push_back(Gdiplus::Color::Cyan);
	m_aColorTab.push_back(Gdiplus::Color::Green);
	m_aColorTab.push_back(Gdiplus::Color::Yellow);
	m_aColorTab.push_back(Gdiplus::Color::Red);
	m_aColorTab.push_back(Gdiplus::Color::Purple);
	//m_aColorTab.push_back(Gdiplus::Color(255, 0, 255));*/
	m_pImage = NULL;
	m_pData = NULL;
}

LDFToPicture::~LDFToPicture()
{
	Free();
	Gdiplus::GdiplusShutdown(m_gdiplusToken);
}


double strech(double omin, double omax, double dmin, double dmax, double val)
{
	return dmin + (val - omin) * (dmax - dmin) / (omax - omin);
}

Gdiplus::Color strechcl(double va, double vb, Gdiplus::Color aColor, Gdiplus::Color bColor, double value)
{

	BYTE r = (BYTE)strech(va, vb, aColor.GetR(), bColor.GetR(), value);
	BYTE g = (BYTE)strech(va, vb, aColor.GetG(), bColor.GetG(), value);
	BYTE b = (BYTE)strech(va, vb, aColor.GetB(), bColor.GetB(), value);
	return Gdiplus::Color(r, g, b);
}

/*
Gdiplus::Color strech(double value)
{
	using namespace Gdiplus;
	Color color1 = Color::Blue;
	Color color2 = Color::Cyan;
	Color color3 = Color::Green;
	Color color4 = Color::Yellow;
	Color color5 = Color::Red;
	Color color6 = Color::Purple;
	//Color color5 = Color::Pink;

	double va1 = -100;
	double va2 = -60;
	double va3 = -20;
	double va4 = 20;
	double va5 = 60;
	double va6 = 100;

	if (value <= va1){
		return color1;
	}
	if (value > va1 && value <= va2){
		return strechcl(va1, va2, color1, color2, value);
	}
	if (value > va2 && value <= va3){
		return strechcl(va2, va3, color2, color3, value);
	}
	if (value > va3 && value <= va4){
		return strechcl(va3, va4, color3, color4, value);
	}
	if (value > va4 && value <= va5){
		return strechcl(va4, va5, color4, color5, value);
	}
	if (value > va5 && value <= va6){
		return strechcl(va5, va6, color5, color6, value);
	}
	if (value > va6){
		return color6;
	}
	return color6;
}*/

Gdiplus::Color LDFToPicture::strechColor(float val)
{
	Gdiplus::Color crst;
	//crst = strechColor(2200, 2900, RGB(0, 0, 0), RGB(255, 255, 255), value);
	if (m_aColorTab.size() >= 2){
		if (val < m_dmin){
			crst = m_aColorTab[0];
		}
		else if (val >= m_dmax)
		{
			crst = m_aColorTab[m_aColorTab.size() - 1];
		}
		else{

			int N = m_aColorTab.size() - 1;
			double step = (m_dmax - m_dmin) / N;
			int ci = (int)((val - m_dmin) / step);

			double va = ci * step + m_dmin;
			double vb = (ci + 1) * step + m_dmin;

			Gdiplus::Color ca = m_aColorTab[ci];
			Gdiplus::Color cb = m_aColorTab[ci + 1];
			crst = strechcl(va, vb, ca, cb, val);
		}
	}
	return crst;
}

int LDFToPicture::Render(CDC* pDC, CRect rc)
{
	ASSERT(pDC);
	if (rc.IsRectNull()) {
		CSize sz = GetImageSize();
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	Gdiplus::Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(Gdiplus::InterpolationModeBilinear);
	//graphics.SetInterpolationMode(Gdiplus::InterpolationModeBicubic);
	graphics.DrawImage(m_pImage, rc.left, rc.top, rc.Width(), rc.Height());
	return 0;
}

int LDFToPicture::Renderlegend(CDC* pDC, CRect rc)
{
	ASSERT(pDC);
	if (rc.IsRectNull()) {
		CSize sz = GetImageSize();
		rc.right = sz.cx;
		rc.bottom = sz.cy;
	}
	using namespace Gdiplus;
	Graphics graphics(pDC->m_hDC);

	SolidBrush solidBrush1(Color(255, 255, 255));
	graphics.FillRectangle(&solidBrush1, rc.left, rc.top, rc.right, rc.bottom);


	Rect rect(rc.left + 20, rc.top, rc.right - 40, rc.top + 12);
	LinearGradientBrush brush(rect, m_aColorTab[0], m_aColorTab[m_aColorTab.size() - 1], LinearGradientModeHorizontal);


	REAL * pos = new REAL[m_aColorTab.size()];
	Color  * cor = new Color[m_aColorTab.size()];

	for (size_t i = 0; i < m_aColorTab.size(); i++){
		pos[i] = ((Gdiplus::REAL) i) / (m_aColorTab.size() - 1);
		cor[i] = m_aColorTab[i];
	}
	brush.SetInterpolationColors(cor, pos, m_aColorTab.size());
	graphics.FillRectangle(&brush, rect);

	
	FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, m_fontsize_palette);//�˴�����ɫ��ͼ�������ȡ�������������ã�����С��
	SolidBrush solidBrush(Color(0, 0, 0));
	Pen pen(Color(0, 0, 0));
	int start = (int)(m_dmin / 10);
	int end = (int)(m_dmax / 10);

	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAliasGridFit);

	for (int i = start; i <= end; i++){
		if (i % 5 == 0 || i == start && (i % 5 <= 3) || i == end && (i % 5 >= 3)){

			int x = rect.X + (i * 10 - m_dmin) * (rect.Width) / (m_dmax - m_dmin);
			graphics.DrawLine(&pen, x, rect.Y + 10, x, rect.Y + 14);
			CString str;
			str.Format(_T("%d"), i);
			graphics.DrawString(str.GetString(), str.GetLength(), &font, PointF(x - 4 * str.GetLength(), rect.Y + 16), &solidBrush);
		}

	}

	
	delete [] pos;
	delete [] cor;

	return 0;

}

void LDFToPicture::setRange(int Channel,double min, double max,int fontsize)
{
	m_Channel=Channel;
	m_dmin = min;
	m_dmax = max;
	//calColor();
	m_fontsize=fontsize;//���þ�γ�������С
	m_fontsize_palette=8;//���õ�ɫ�������С
	//���ݾ�γ�������С�����û�ͼ������Χ���ͱ߿�Ĵ�С
	m_Width_top=int(fontsize);
	m_Width_bottom=int(fontsize*1.5)+5;
	m_Width_right=int(fontsize*1.5)+5;
	m_Width_left=int(fontsize*2.3);
}

void LDFToPicture::setColor(std::vector<Gdiplus::Color>& colorTab)
{
	m_aColorTab.clear();
	for (size_t i = 0; i < colorTab.size(); i++){
		m_aColorTab.push_back(colorTab[i]);
	}
	calColor();
}


CSize LDFToPicture::GetImageSize() const
{
	CSize sz(0, 0);
	if (m_pImage) {
		sz = CSize(m_pImage->GetWidth(), m_pImage->GetHeight());
	}
	return sz;
}

void LDFToPicture::Free()
{
	if (m_pImage) {
		delete m_pImage;
		m_pImage = NULL;
	}
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	m_nSample = 0; 
	m_nLine = 0;
}

/*
	����ͼ��Ĺ��̣�
	1���õ�������ͼ��Ŀ�ߡ��Լ���γ�ȷ�Χ����γ�ȷֱ���
	2���õ����Ͻǵľ�γ�ȣ�������Ͻ���ָpictrue control�����Ͻǣ���ԭͼ�����ϽǾ�γ�ȼ�ȥ�հױߵõ���
	3���½�һ������m_pData�Դ��Ӱ�񣨰��������鿽�������������飩
	4��createImage()����һ��������m_pImage��������Ӱ�������ܣ���GDI+��������ͼ
	5��calColor()����m_pData�����α��ɫͼ�����飬������m_pImage�м䣨��Ҫ��һ��ƫ������m_Width_left, m_Width_top���ģ�
*/
int LDFToPicture::setLDF(LDF& ldf)
{
	Free();
	m_nSample = ldf.m_hdr.wWidth;
	m_nLine = ldf.m_hdr.wHeight;
	m_dMinLon = ldf.m_hdr.fMinLon;
	m_dMaxLon = ldf.m_hdr.fMaxLon;
	m_dMinLat = ldf.m_hdr.fMinLat;
	m_dMaxLat = ldf.m_hdr.fMaxLat;
	m_dratioX = 1 / ldf.m_hdr.fLonSolution;
	m_dratioY = 1 / ldf.m_hdr.fLatSolution;

	m_origin = Point2D(m_dMinLon - m_Width_left / m_dratioX, m_dMaxLat + m_Width_top / m_dratioY);


	m_pData = new float[m_nSample * m_nLine];

	for (int i = 0; i < m_nLine; i++){
		for (int j = 0; j < m_nSample; j++){
			m_pData[i * m_nSample + j] = ldf.getPixel(m_Channel, j, i);
		}
	}
	//����ÿ��ͼ�񣨻���˵ldf������ʾ��Χ������������Ϊ�����Сֵ
	//setRange(m_Channel,m_dmin, m_dmax,m_fontsize);//�Ѹ�Ϊ����setLDFǰֱ�ӵ���
	createImage();//ldf);
	//drawedge(ldf);//�Ѹ�Ϊ����setLDFǰֱ�ӵ��ã����ڻ�Ҫ��ʸ��ͼ�㣩
	calColor();
	
	return 0;
}

//�������ϲ����������ƾͲ����ˣ�float *KValue��float *Mean
int LDFToPicture::setTIFF(LDF& ldf,float *KValue)
{
	Free();
	m_nSample = ldf.m_hdr.wWidth;
	m_nLine = ldf.m_hdr.wHeight;
	m_dMinLon = ldf.m_hdr.fMinLon;
	m_dMaxLon = ldf.m_hdr.fMaxLon;
	m_dMinLat = ldf.m_hdr.fMinLat;
	m_dMaxLat = ldf.m_hdr.fMaxLat;
	m_dratioX = 1 / ldf.m_hdr.fLonSolution;
	m_dratioY = 1 / ldf.m_hdr.fLatSolution;

	m_origin = Point2D(m_dMinLon - m_Width_left / m_dratioX, m_dMaxLat + m_Width_top / m_dratioY);


	m_pData = new float[m_nSample * m_nLine];

	for (int i = 0; i < m_nLine; i++){
		for (int j = 0; j < m_nSample; j++){
			m_pData[i * m_nSample + j] = KValue[i*m_nSample+j];
		}
	}
	//���ñ�������ֵ������˵tiff������ʾ��Χ������������Ϊ�����Сֵ
	//����ÿ��Kֵ������˵tiff������ʾ��Χ������������Ϊ�����Сֵ
	//setRange(m_Channel,m_dmin, m_dmax,m_fontsize);//�Ѹ�Ϊ����setTIFFǰֱ�ӵ���
	createImage();//ldf);
	//drawedge(ldf);//�Ѹ�Ϊ����setTIFFǰֱ�ӵ��ã����ڻ�Ҫ��ʸ��ͼ�㣩
	calColor();
	
	return 0;
}

/*
int LDFToPicture::setMEAN(LDF& ldf,float *Mean)
{
	Free();
	m_nSample = ldf.m_hdr.wWidth;
	m_nLine = ldf.m_hdr.wHeight;
	m_dMinLon = ldf.m_hdr.fMinLon;
	m_dMaxLon = ldf.m_hdr.fMaxLon;
	m_dMinLat = ldf.m_hdr.fMinLat;
	m_dMaxLat = ldf.m_hdr.fMaxLat;
	m_dratioX = 1 / ldf.m_hdr.fLonSolution;
	m_dratioY = 1 / ldf.m_hdr.fLatSolution;

	m_origin = Point2D(m_dMinLon - m_Width / m_dratioX, m_dMaxLat + m_Width_l / m_dratioY);


	m_pData = new float[m_nSample * m_nLine];

	for (int i = 0; i < m_nLine; i++){
		for (int j = 0; j < m_nSample; j++){
			m_pData[i * m_nSample + j] = Mean[i*m_nSample+j];
		}
	}
	//���ñ�������ֵ������˵tiff������ʾ��Χ������������Ϊ�����Сֵ
	setRange(m_dmin/10, m_dmax/10);
	createImage();//ldf);
	drawedge(ldf);
	calColor();
	
	return 0;
}
*/

int LDFToPicture::calColor()
{
	for (int i = 0; i < m_nLine; i++){
		for (int j = 0; j < m_nSample; j++){
			//�� m_pData(i,j) �ĵ�õ� m_pImage(x,y) ����ɫ
			//m_pData��һά���飬i+1 �� j+1 �� ��������i * m_nSample + j���ö�ά�����ʾΪm_pData[i][j]
			//m_pImage��Ӧ��Ļ���꣨x��y����x��ָ�����У�y��ָ�����У��볣���෴
			//����������ͼ�հ�����Ĵ��ڣ�����֮�仹����ƫ����
			//���߹�ϵΪ��m_pImage[y][x] <=> m_pData[y - m_Width_left][x - m_Width_top]
			//���߱�ʾΪ��m_pImage[i + m_Width_top][x + m_Width_left] <=> m_pData[i][j]
			m_pImage->SetPixel(j + m_Width_left, i + m_Width_top, strechColor(m_pData[i * m_nSample + j]));
		}
	}
	return 0;
}

//����һ��������m_pImage������С��ͼ��������ܰױ�
BOOL LDFToPicture::createImage()//LDF& ldf)
{
	m_pImage = new Gdiplus::Bitmap(m_nSample + m_Width_left+m_Width_right, m_nLine + m_Width_top+m_Width_bottom);
	if (!m_pImage){
		return FALSE;
	}
	Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 255, 255));
	Gdiplus::Graphics graphics(m_pImage);
	graphics.FillRectangle(&solidBrush, 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight());
	
	return TRUE;
}

//����ͼ�����ܰױ����򣬾��Ǿ�γ�����ֲ���
int LDFToPicture::drawedge(LDF& ldf)
{
	if (!m_pImage){
		return 1;
	}
	
	Gdiplus::Graphics graphics(m_pImage);
	Gdiplus::Pen pen(Gdiplus::Color::Black,2.0);//�����߿򼰿̶ȱ�ע�Ļ���
	Gdiplus::FontFamily fontFamily(L"Arial");
	Gdiplus::Font font(&fontFamily, m_fontsize);//��澭γ�ȱ�ע�Ļ���
	Gdiplus::SolidBrush whitebrush(Gdiplus::Color(255, 255, 255));//��ɫ������߿������������д���Ϊ���
	//��ͼʱ����������������������޸İ�ɫ������߿�Ŀ��
	graphics.FillRectangle(&whitebrush, 0, 0, m_pImage->GetWidth(), m_Width_top);//�ϣ�ע���ʱm_pImageָ��������ͼ�񣬼�������ɫ��߿�
	graphics.FillRectangle(&whitebrush, 0, m_pImage->GetHeight() - m_Width_bottom, m_pImage->GetWidth(), m_Width_bottom);//��
	graphics.FillRectangle(&whitebrush, 0, 0, m_Width_left, m_pImage->GetHeight());//��
	graphics.FillRectangle(&whitebrush, m_pImage->GetWidth() - m_Width_right, 0, m_Width_right, m_pImage->GetHeight());//��

	Gdiplus::SolidBrush brush(Gdiplus::Color(0, 0, 0));//��ͼ����ĺ�ɫ������߿�
	graphics.DrawRectangle(&pen, m_Width_left-1, m_Width_top-1, m_pImage->GetWidth() - m_Width_left-m_Width_right+1, m_pImage->GetHeight() - m_Width_top-m_Width_bottom+1);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHint::TextRenderingHintAntiAliasGridFit);

	int x1, y1;
	CString str;

	/******************************************
	(int(ceil(m_dMinLon))+m_step-1)/m_step*m_step
	�㷨Ŀ�ģ�
	ʹ m_dMinLonΪ( 0, 5]ʱ���õ� 5
	ʹ m_dMinLonΪ( 5,10]ʱ���õ�10
	ʹ m_dMinLonΪ(10,15]ʱ���õ�15
	��������������������������������

	�㷨˼�룺
	��X����( 5,10]Ϊ��
	int(ceil(X))	��X��( 5,10]ӳ�䵽{6,7,8,9,10}
	(Y+4)/5*5		��Y��{6,7,8,9,10}ӳ�䵽10
	*********************************************/
	for (int i = (int(ceil(m_dMinLon))+m_step-1)/m_step*m_step; i <= (int)m_dMaxLon; i=i+m_step){
		
		ldf.geo2pix(i, m_dMaxLat, x1, y1);//ͼ���ϱ߽�Ϊ���γ��
		x1 += m_Width_left;
		y1 += m_Width_top;
		graphics.DrawLine(&pen, x1, y1 + m_fontsize/3, x1, y1 - 3);//���������С���ÿ̶ȳ���
		
		str.Format(_T("%d��"), i);
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - 7 * str.GetLength(), y1 - 24), &brush);//��
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - m_fontsize/2 * str.GetLength(), y1 - m_fontsize *1.3 - 5), &brush);
		ldf.geo2pix(i, m_dMinLat, x1, y1);//ͼ���±߽�Ϊ��Сγ��
		x1 += m_Width_left;
		y1 += m_Width_top;
		graphics.DrawLine(&pen, x1, y1 + 3, x1, y1 - m_fontsize/3);//���������С���ÿ̶ȳ���
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - 7 * str.GetLength(), y1 + 5), &brush);//��
		if(i-m_dMinLon>0.5 && m_dMaxLon-i>0.5)
			graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - m_fontsize/2 * (str.GetLength()-1), y1 + 5), &brush);

		/*//����ж��ǲ���Ҫ�����Ͻ����һ��γ�ȿ̶�
		if ((i+m_step)>(int)m_dMaxLon)//�ж��ǲ������һ��i
		{
			if(i<(m_dMaxLon-3))//�ж����һ����ǵ�γ�Ⱦ������Ͻ�γ�ȳ���3��
			{
				/////////////////�໭һ�����Ͻ�γ��
				ldf.geo2pix(m_dMaxLon, m_dMinLat, x1, y1);
				x1 += m_Width_left;
				y1 += m_Width_top;
				str.Format(_T("%d��"),(int)m_dMaxLon);
				graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - m_fontsize/2 * (str.GetLength()-1), y1 + 5), &brush);
			}
		}
		else
		{
			continue;
		}*/
	}


	for (int i = (int(ceil(m_dMinLat))+m_step-1)/m_step*m_step; i <= (int)m_dMaxLat; i=i+m_step){
		ldf.geo2pix(m_dMinLon, i, x1, y1);
		x1 += m_Width_left;
		y1 += m_Width_top;
		graphics.DrawLine(&pen, x1 - 3, y1, x1 + m_fontsize/3, y1);//���������С���ÿ̶ȳ���
		str.Format(_T("%d��"), i);
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - 16 * str.GetLength(), y1 - 12), &brush);//��
		if(i-m_dMinLat>0.5 && m_dMaxLat-i>0.5)
			graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - m_fontsize*0.75 * str.GetLength()-5, y1 - m_fontsize*0.9), &brush);

		ldf.geo2pix(m_dMaxLon, i, x1, y1);
		x1 += m_Width_left;
		y1 += m_Width_top;
		graphics.DrawLine(&pen, x1 - m_fontsize/3, y1, x1 + 3, y1);//���������С���ÿ̶ȳ���
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 + 5, y1 - 12), &brush);//��
		//graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1-5 , y1 - m_fontsize - 5), &brush);

		/*/����ж��ǲ���Ҫ�����½����һ�����ȿ̶�
		if ((i+m_step)>(int)m_dMaxLat)//�ж��ǲ������һ��i
		{
			if(i<(m_dMaxLat-3))//�ж����һ����ǵľ��Ⱦ������½Ǿ��ȳ���3��
			{
				/////////////////�໭һ�����½Ǿ���
				ldf.geo2pix(m_dMinLon, m_dMaxLat, x1, y1);
				x1 += m_Width_left;
				y1 += m_Width_top;
				str.Format(_T("%d��"),(int)m_dMaxLat);
				graphics.DrawString(str.GetString(), str.GetLength(), &font, Gdiplus::PointF(x1 - m_fontsize*0.75 * str.GetLength(), y1 - m_fontsize*0.8), &brush);//ϵ��Ϊ0.8����΢����
			}
		}
		else
		{
			continue;
		}*/

	}

	return 0;
}

//����ͳ������
int LDFToPicture::draw_StatisticArea(LDF& ldf, IProvider * pProvider,int style,COLORREF col,float Lon,float Lat,float Width,float Height)
{
	if (!m_pImage){
		return 1;
	}
	
	Gdiplus::Graphics graphics(m_pImage);
	graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
	//GDI+����ɫò���ǵ���ģ�BGRA������͸����Ҳ��Ҫ��255��
	Gdiplus::SolidBrush transbrush(Gdiplus::Color(255-(BYTE) (col >> 24),(BYTE) (col),(BYTE) (col >> 8),(BYTE) (col >> 16)));

	/*��֪��Ϊʲô0x880000ff���ԣ�colȴ���С�������
	Gdiplus::SolidBrush sb(Gdiplus::Color(0x880000ff));
	graphics.FillRectangle(&sb, 100,1000,100,100);//
	*/

	int x1, y1, x2, y2;
	ldf.geo2pix(Lon-Width, Lat+Height, x1, y1);//�������Ͻ�����
	ldf.geo2pix(Lon+Width, Lat-Height, x2, y2);//�������½�����

	//����������ͳ���������˼����Ұױ���
	x1 += m_Width_left;
	y1 += m_Width_top;
	x2 += m_Width_left;
	y2 += m_Width_top;

	switch (style)
	{
		case 0://����Բ������
		{
			graphics.FillEllipse(&transbrush, x1, y1, x2-x1, y2-y1);
		}
			break;
		case 1://���ƾ�������
		{
			graphics.FillRectangle(&transbrush, x1, y1, x2-x1, y2-y1);
		}
			break;
		case 2://���ƶ������⣬��Ϊ���ص������ԾͲ�����͸����
		{
			int fault_in = 20;//ħ�������Ѵ��ڲ�����λ������
			int fault_out = 50;//ħ�������Ѵ���Χ����λ������
			std::vector<const shp::ShapeObject *> vecShp = pProvider->getGeometries();

			//���ƶ��Ѵ���Χ
			Gdiplus::Pen pen_out(Gdiplus::Color(255,255-(BYTE) (col),255-(BYTE) (col >> 8),255-(BYTE) (col >> 16)),fault_out);
			Gdiplus::SolidBrush brush_out(Gdiplus::Color(255,255-(BYTE) (col),255-(BYTE) (col >> 8),255-(BYTE) (col >> 16)));
			for (IProvider::ITER_SHP iter = vecShp.begin();
				iter < vecShp.end();
				iter++)
			{
				const shp::ShapeObject * pShp = *iter;
				switch (pShp->ShapeType)
				{
					case shp::ShapeType::PolyLine:
					{
						const shp::ShpPolyLine& pl = *static_cast<const shp::ShpPolyLine *>(pShp);
						for (int i = 0; i<pl.NumParts; ++i)
						{
							int count = 0;
							const shp::Point* pts = pl.getPart(i, &count);
							POINT pt;
							Gdiplus::Point * gpts = new Gdiplus::Point[count];
							for (int j = 0; j<count; ++j)
							{
								this->GeoToMap(pts[j].X, pts[j].Y, &pt);
								gpts[j] = Gdiplus::Point(pt.x, pt.y);
							}
							graphics.DrawLines(&pen_out, gpts, count);

							//���⣬Ϊ�˺ÿ���ҲΪ׼ȷ������β������Ҫ��һ��Բ��
							//////////////////////////////////////////////////////////////////////
							graphics.FillEllipse(&brush_out, gpts[0].X-fault_out/2, gpts[0].Y-fault_out/2, fault_out, fault_out);
							graphics.FillEllipse(&brush_out, gpts[count-1].X-fault_out/2, gpts[count-1].Y-fault_out/2, fault_out, fault_out);
							//////////////////////////////////////////////////////////////////////

							delete[] gpts;

						}
					}
					break;
				}
			}
			//���ƶ��Ѵ���Χ����
			
			//���ƶ��Ѵ��ڲ�
			Gdiplus::Pen pen_in(Gdiplus::Color(255,(BYTE) (col),(BYTE) (col >> 8),(BYTE) (col >> 16)),fault_in);
			Gdiplus::SolidBrush brush_in(Gdiplus::Color(255,(BYTE) (col),(BYTE) (col >> 8),(BYTE) (col >> 16)));
			for (IProvider::ITER_SHP iter = vecShp.begin();
				iter < vecShp.end();
				iter++)
			{
				const shp::ShapeObject * pShp = *iter;
				switch (pShp->ShapeType)
				{
					case shp::ShapeType::PolyLine:
					{
						const shp::ShpPolyLine& pl = *static_cast<const shp::ShpPolyLine *>(pShp);
						for (int i = 0; i<pl.NumParts; ++i)
						{
							int count = 0;
							const shp::Point* pts = pl.getPart(i, &count);
							POINT pt;
							Gdiplus::Point * gpts = new Gdiplus::Point[count];
							for (int j = 0; j<count; ++j)
							{
								this->GeoToMap(pts[j].X, pts[j].Y, &pt);
								gpts[j] = Gdiplus::Point(pt.x, pt.y);
							}
							graphics.DrawLines(&pen_in, gpts, count);

							//���⣬Ϊ�˺ÿ���ҲΪ׼ȷ������β������Ҫ��һ��Բ��
							//////////////////////////////////////////////////////////////////////
							graphics.FillEllipse(&brush_in, gpts[0].X-fault_in/2, gpts[0].Y-fault_in/2, fault_in, fault_in);
							graphics.FillEllipse(&brush_in, gpts[count-1].X-fault_in/2, gpts[count-1].Y-fault_in/2, fault_in, fault_in);
							//////////////////////////////////////////////////////////////////////

							delete[] gpts;
						}
					}
					break;
				}
			}
			//���ƶ��Ѵ��ڲ�����

		}
			break;
	}
	return 0;
}

int LDFToPicture::LDF2Bmp(LDF& ldf)
{

	return 0;
}

int GetImageCLSID(const   WCHAR*   format, CLSID*   pCLSID)
{
	//�õ���ʽΪformat��ͼ���ļ��ı���ֵ�����ʸø�ʽͼ���COM�����GUIDֵ������pCLSID�� 
	UINT num = 0;
	UINT size = 0;

	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);

	if (size == 0)
		return FALSE;   //   ������Ϣ������ 

	//�����ڴ� 
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return FALSE;   //   ����ʧ�� 

	//���ϵͳ�п��õı��뷽ʽ��������Ϣ 
	GetImageEncoders(num, size, pImageCodecInfo);

	//�ڿ��ñ�����Ϣ�в���format��ʽ�Ƿ�֧�� 
	for (UINT i = 0; i < num; ++i)
	{
		//MimeType�����뷽ʽ�ľ������� 
		if (wcscmp(pImageCodecInfo[i].MimeType, format) == 0)
		{
			*pCLSID = pImageCodecInfo[i].Clsid;
			free(pImageCodecInfo);
			return TRUE;
		}
	}

	free(pImageCodecInfo);
	return FALSE;
}

int LDFToPicture::Save(CString filename)
{
	CLSID clImageClsid;
	GetImageCLSID(_T("image/png"), &clImageClsid);
	m_pImage->Save(filename, &clImageClsid);
	return 0;
}

/*������DrawMap
�������⣺exception�����������ʱ����ʾ����ǵĴ�С��
���û�����ɫ��SetColor
���û��ʿ�ȣ�pen_width
Gdiplus::Color::White=-1,Black=-16777216,red=-65536
*/
void LDFToPicture::DrawMap_arg(IProvider * pProvider,Gdiplus::Color Color_arg,int pen_width,int exception)
{
	// draw

	//CBrush	NewBrush;
	//CBrush *OldBrush;
	//NewBrush.CreateSolidBrush(RGB(255, 0, 0));
	//OldBrush = pDC->SelectObject(&NewBrush);


	//exception����1����ζ��pen_width��ʾ���д�С���������߿�
	/*����������ôд�����뱨��error C2065: ��pen��: δ�����ı�ʶ��
	if (exception==1)
		Gdiplus::Pen pen(Color_arg,pen_width);
	else
		Gdiplus::Pen pen(Gdiplus::Color::White,3);//ħ�������Ϊ3�İ��ߣ�Ҫ�����������Χ
	*/

	Gdiplus::Graphics graph(m_pImage);
	graph.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);

	std::vector<const shp::ShapeObject *> vecShp = pProvider->getGeometries();

	switch (exception)
	{
	case 1:
		{
			Gdiplus::Pen pen(Color_arg,pen_width);
			for (IProvider::ITER_SHP iter = vecShp.begin();
				iter < vecShp.end();
				iter++)
			{
				const shp::ShapeObject * pShp = *iter;
				switch (pShp->ShapeType)
				{
				case shp::ShapeType::Point:
				{
					const shp::ShpPoint &shpPt = *static_cast<const shp::ShpPoint *>(pShp);
					POINT pt;
					this->GeoToMap(shpPt.X, shpPt.Y, &pt);
					graph.DrawEllipse(&pen, pt.x - 5, pt.y + 5, 10, 10);
					//pDC->FloodFill( pt.x, pt.y, RGB(255, 0, 0) );
				}
					break;
				case shp::ShapeType::Multipoint:
				{
					const shp::ShpMultiPoint &mpt = *static_cast<const shp::ShpMultiPoint *>(pShp);
					POINT pt;
					for (int i = 0; i<mpt.NumPoints; ++i)
					{
						this->GeoToMap(mpt.Points[i].X, mpt.Points[i].Y, &pt);
						graph.DrawEllipse(&pen, pt.x - 5, pt.y + 5, 10, 10);
					}
				}
					break;
				case shp::ShapeType::PolyLine:
				{
					const shp::ShpPolyLine& pl = *static_cast<const shp::ShpPolyLine *>(pShp);
					for (int i = 0; i<pl.NumParts; ++i)
					{
						int count = 0;
						const shp::Point* pts = pl.getPart(i, &count);
						POINT pt;
						Gdiplus::Point * gpts = new Gdiplus::Point[count];
						for (int j = 0; j<count; ++j)
						{
							this->GeoToMap(pts[j].X, pts[j].Y, &pt);
							gpts[j] = Gdiplus::Point(pt.x, pt.y);
						}
						//graph.DrawPolygon(&pen, gpts, count);
						graph.DrawLines(&pen, gpts, count);
						delete[] gpts;
					}
				}
					break;
				case shp::ShapeType::Polygon:
				{
					const shp::ShpPolygon& polygon = *static_cast<const shp::ShpPolygon *>(pShp);
					int NumParts = polygon.NumParts;
					int NumPoints = polygon.NumPoints;
					int * counts = new int[polygon.NumParts];
					POINT * pts = new POINT[polygon.NumPoints];

					this->GeoToMap(&polygon.Points(0), pts, polygon.NumPoints);
					for (int i = 0; i<polygon.NumParts - 1; ++i)
						counts[i] = polygon.Parts[i + 1] - polygon.Parts[i];
					counts[polygon.NumParts - 1] = polygon.NumPoints - polygon.Parts[polygon.NumParts - 1];

					//CRgn rgn;
					//rgn.CreatePolyPolygonRgn(pts, counts, polygon.NumParts, WINDING);
					//CBrush fillBrush;
					//fillBrush.CreateSolidBrush(RGB(0, 255, 0));
					//pDC->FillRgn(&rgn, &fillBrush);
					//fillBrush.DeleteObject();

					delete counts;
					delete[] pts;
					Gdiplus::Point * gpts;
					for (int i = 0; i<polygon.NumParts; ++i)
					{
						int count = 0;
						const shp::Point* pts = polygon.getPart(i, &count);
						POINT pt;
						gpts= new Gdiplus::Point [count];
						for (int j = 0; j<count; ++j){
							this->GeoToMap(pts[j].X, pts[j].Y, &pt);
							gpts[j] = Gdiplus::Point(pt.x, pt.y);
						}
						graph.DrawPolygon(&pen, gpts, count);
						delete [] gpts;
					}
				}
					break;
				}
			}

			//pDC->SelectObject(OldBrush);
			//NewBrush.DeleteObject();

		}
		break;
	default://����
		{
			Gdiplus::Pen pen(Gdiplus::Color::White,3);//ħ�������Ϊ3�İ��ߣ�Ҫ�����������Χ
			for (IProvider::ITER_SHP iter = vecShp.begin();
				iter < vecShp.end();
				iter++)
			{
				const shp::ShapeObject * pShp = *iter;
				switch (pShp->ShapeType)
				{
				case shp::ShapeType::Point:
				{
					Gdiplus::SolidBrush brush(Color_arg);//�������ɫ
					const shp::ShpPoint &shpPt = *static_cast<const shp::ShpPoint *>(pShp);
					POINT pt;
					this->GeoToMap(shpPt.X, shpPt.Y, &pt);
					//***********************���㻭�������************************
					float rad;
					int t;
					Gdiplus::Point * gpts = new Gdiplus::Point[10];
					for (int j = 0; j<10; ++j)
					{
						rad = (j * 36 + 18)* 3.1415926536 / 180;;
						t = j & 1 ? pen_width : pen_width/2;//exception����1����ζ��pen_width��ʾ���д�С���������߿�
						gpts[j] = Gdiplus::Point(pt.x+t * cos(rad), pt.y+t * sin(rad));
					}
					graph.FillPolygon(&brush, gpts, 10);
					graph.DrawPolygon(&pen, gpts, 10);
					delete[] gpts;
					/**************************************************************/
					//graph.DrawEllipse(&pen, pt.x - 5, pt.y + 5, 10, 10);// be annotated by LiuAnming
				}
				}
			}
		}
	}	
}

void LDFToPicture::GeoToMap(const shp::Point * geoPts, POINT * pts, int count)
{
	for (int i = 0; i<count; ++i)
	{
		pts[i].x = (int)((geoPts[i].X - m_origin.X) * m_dratioX);
		pts[i].y = (int)((m_origin.Y - geoPts[i].Y) * m_dratioY);
	}
}

void LDFToPicture::GeoToMap(double geoX, double geoY, POINT * pt)
{
	pt->x = (int)((geoX - m_origin.X) * m_dratioX);
	pt->y = (int)((m_origin.Y - geoY) * m_dratioY);
}

void LDFToPicture::MapToGeo(int x, int y, Point2D * geoPoint)
{
	geoPoint->X = (x / m_dratioX) + m_origin.X;
	geoPoint->Y = m_origin.Y - (y / m_dratioY);
}