#include "StdAfx.h"
#include "LDF.h"
#include <fstream>
#include <math.h>
#include <iostream>

LDF::LDF(void)
{
	m_Band = NULL;
}

LDF::~LDF(void)
{

	if(m_Band != NULL){
		for(int i = 0; i < 5; i++){
			delete [] m_Band[i];
		}
		delete [] m_Band;
	}
}

bool LDF::readHdr(CString filepath)
{
	std::locale prev_loc = std::locale::global( std::locale("chs") ); 
	std::ifstream infile;
	infile.open(filepath, std::ios::binary);
	std::locale::global( prev_loc );

	if(!infile){
		return false;
	}
	memset((void *)&m_hdr, 0, sizeof(m_hdr));
	infile.read((char*)&m_hdr, sizeof(m_hdr));
	infile.close();

	if (m_hdr.wWidth == 0 || m_hdr.wHeight == 0){
		return false;
	}
	return true;
}


bool LDF::read(CString filepath)
{
	std::locale prev_loc = std::locale::global( std::locale("chs") ); 
	std::ifstream infile;
	infile.open(filepath, std::ios::binary);
	std::locale::global( prev_loc );

	if(!infile){
		return false;
	}
	if(m_Band != NULL){
		for(int i = 0; i < 5; i++){
			delete [] m_Band[i];
		}
		delete [] m_Band;
	}

	memset((void *)&m_hdr, 0, sizeof(m_hdr));
	infile.read((char*)&m_hdr, sizeof(m_hdr));
	if (m_hdr.wWidth == 0 || m_hdr.wHeight == 0){
		return false;
	}
	m_Sample = m_hdr.wWidth;
	m_Line = m_hdr.wHeight;
	m_Band = new short * [5];
	for(int i = 0; i < 5; i++){
		m_Band[i] = new short [m_Sample * m_Line];
	}

	////由格林尼治时间转换为东八区时间
	//if(hdr.wHour + 8 >= 24){
	//	hdr.wHour = hdr.wHour + 8 - 24;
	//	hdr.wDay = hdr.wDay + 1;
	//}
	//else{
	//	hdr.wHour = hdr.wHour + 8;
	//}

	if(m_hdr.wChannelNums == 3){
		for(unsigned short i = 2; i < 5; i++){
			infile.read((char*)m_Band[i], m_Sample * m_Line * sizeof(unsigned short));
		}
	}
	if(m_hdr.wChannelNums == 5){
		for(unsigned short i = 0; i < 5; i++){
			infile.read((char*)m_Band[i], m_Sample * m_Line * sizeof(unsigned short));
		}
	}
	infile.close();
	return true;
}

int LDF::write(CString filepath)
{
	if(filepath.GetLength() == 0){
		return -1;
	}

	std::locale prev_loc = std::locale::global( std::locale("chs") ); 
	std::ofstream outfile;
	outfile.open(filepath, std::ios::binary);
	std::locale::global( prev_loc );

	if(!outfile){
		return -2;
	}
	outfile.write((char*)&m_hdr, sizeof(m_hdr));
	for(int i = 0; i < 5; i++){
		outfile.write((char*)m_Band[i], sizeof(unsigned short) * m_Sample * m_Line);
	}
	outfile.close();
	return 0;
}

//计算指定地理位置，指定窗口大小的像素均值，零值（无效值215K到350K之外）不参与计算
double LDF::getWndMean(int b, double x, double y, int nSize)
{
	int cx = int((x - m_hdr.fMinLon) / m_hdr.fLonSolution + 0.5);
	int cy = int((m_hdr.fMaxLat - y) / m_hdr.fLatSolution + 0.5);

	int sx, sy, ex, ey;
	if(nSize % 2 == 0){
		sx = cx - nSize / 2 + 1;
		sy = cy - nSize / 2 + 1;
		ex = cx + nSize / 2;
		ey = cy + nSize / 2;
	}else{
		sx = cx - nSize / 2;
		sy = cy - nSize / 2;
		ex = cx + nSize / 2;
		ey = cy + nSize / 2;
	}

	double Mean = 0;
	int cnt = 0;
	for(int ix = sx; ix <= ex; ix++){
		for(int iy = sy; iy <= ey; iy++){
			double val = getPixel(b, ix, iy);
			if( val > 2000 && val < 3500){
				Mean += val;
				cnt ++;
			}
		}
	}
	if(0 != cnt){
		return Mean / cnt;
	}else{
		return 0;
	}
}

double checkExtent(double ax1, double ay1, double ax2, double ay2,
	double bx1, double by1, double bx2, double by2)
{
	double Xc1 = max(ax1, bx1);
	double Yc1 = max(ay1, by1);
	double Xc2 = min(ax2, bx2);
	double Yc2 = min(ay2, by2);

	if (Xc1 <= Xc2 && Yc1 <= Yc2){
		return (Xc2 - Xc1) * (Yc2 - Yc1) / (ax2 - ax1) / (ay2 - ay1);
	}
	else{
		return 0.;
	}
}


double checkGeoExtent(LocalFileHeader hdr1, LocalFileHeader hdr2)
{
	float ax1 = hdr1.fMinLon;
	float ay1 = hdr1.fMinLat;
	float ax2 = hdr1.fMaxLon;
	float ay2 = hdr1.fMaxLat;
	float bx1 = hdr2.fMinLon;
	float by1 = hdr2.fMinLat;
	float bx2 = hdr2.fMaxLon;
	float by2 = hdr2.fMaxLat;

	return checkExtent(ax1, ay1, ax2, ay2, bx1, by1, bx2, by2);
}




int noCloudLdfBlend(LDF& ldf1, LDF& ldf2, int min, int max)
{
	ldf1.getPixel(1, 0, 0);
	for (int iy= 0; iy < ldf1.m_Line; iy++){
		for (int ix = 0; ix < ldf1.m_Sample; ix++){
			double gx, gy;
			//int sx = 0, sy = 0;
			ldf1.pix2geo(ix, iy, gx, gy);
			//ldf1.geo2pix(gx, gy, sx, sy);


			for (int ib = 3; ib <= 5; ib++){
				unsigned short t1 = ldf1.getPixel(ib, ix, iy);
				unsigned short t2 = ldf2.getPixelGeo(ib, gx, gy);
				if (t2 > t1 && t2 < max){
					ldf1.setPixel(ib, ix, iy, t2);
				}
				/*
				if (t2 > t1 && t2 > min && t2 < max){
					ldf1.setPixel(ib, ix, iy, t2);
				}*/
			}
		}
	}
	return 0;
}

int noCloudLdfBlend(LDF& ldf1, LDF& ldf2, int min, int max, 
	double Xmin, double Xmax, double Xres,
	double Ymin, double Ymax, double Yres)
{
	ldf1.getPixel(1, 0, 0);
	for (int iy = 0; iy < ldf1.m_Line; iy++){
		for (int ix = 0; ix < ldf1.m_Sample; ix++){
			double gx, gy;
			ldf1.pix2geo(ix, iy, gx, gy);
			for (int ib = 3; ib < 5; ib++){
				unsigned short t1 = ldf1.getPixel(ib, ix, iy);
				unsigned short t2 = ldf2.getPixelGeo(ib, ix, iy);
				if (t2 > t1 && t2 < max){
					ldf1.setPixel(ib, ix, iy, t2);
				}
			}
		}
	}
	return 0;
}


void LDFSubtraction(LDF& ldf1, LDF& ldf2, short min, short max)
{
	int sample = ldf1.m_hdr.wWidth;
	int line = ldf1.m_hdr.wHeight;
	for(int i = 0; i < line; i++){
		for(int j = 0; j < sample; j++){
			double gx, gy;
			ldf1.pix2geo(j, i, gx, gy);
			for(int k = 3; k <= 5; k++){
				short v1 = ldf1.getPixel(k, j, i);
				short v2 = ldf2.getPixelGeo(k, gx, gy);
				if(v1 >= min && v1 <= max && v2 >= min && v2 <= max){
					ldf1.setPixel(k, j, i, v1 - v2);
				}
				else{
					ldf1.setPixel(k, j, i, 0);
				}
			}
		}
	}
}