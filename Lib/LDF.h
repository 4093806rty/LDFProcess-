#pragma once
#include<string>
#include<afx.h>
#include<vector>


struct LocalFileHeader
{
	char wFileID[2]; // 局地文件标识,固定为字符串"LA" 
	unsigned short wSatelliteID; // 卫星标识, NOAA-14: 14, NOAA-12: 12
	unsigned short wOrbitNums; // 轨道号
	unsigned short wUporDown; // 升降轨标记, 1: 升轨, 0: 降轨

	unsigned short wYear; // 年
	unsigned short wMonth; // 月
	unsigned short wDay; // 日
	unsigned short wHour; // 时
	unsigned short wMinute; // 分

	unsigned short wDayorNight; // 白天黑夜标识, 0: 白天, 1: 黑夜
	unsigned short wChannelNums; // 通道数, 白天: 5, 黑夜: 3

	unsigned short wProjectType; // 投影方式, 0: 不投影, 1: 等角投影
	// 2: 麦卡托投影, 3: 兰布托投影
	// 4: 极射赤面投影

	unsigned short wWidth; // 列数
	unsigned short wHeight; // 行数

	float fLonSolution; // 经度分辨率 -- 等角投影
	//或x分辨率 -- 麦卡托、兰布托、极射赤面投影 
	float fLatSolution; // 纬度分辨率 -- 等角投影
	//或y分辨率 -- 麦卡托、兰布托、极射赤面投影

	float fStandardLat1; //标准纬度1--麦卡托、兰布托、极射赤面投影有效
	float fStandardLat2; //标准纬度2 -- 兰布托投影有效
	float fEarthR; // 地球半径--麦卡托、兰布托、极射赤面投影有效

	float fMinLat; // 最小纬度
	float fMaxLat; // 最大纬度
	float fMinLon; // 最小经度
	float fMaxLon; // 最大经度

	float fLBSunAngle; // 左下角太阳天顶角
	float fLTSunAngle; // 左上角太阳天顶角
	float fRBSunAngle; // 右下角太阳天顶角
	float fRTSunAngle; // 右上角太阳天顶角

	float fLBSateAngle; // 左下角卫星天顶角
	float fLTSateAngle; // 左上角卫星天顶角
	float fRBSateAngle; // 右下角卫星天顶角
	float fRTSateAngle; // 右上角卫星天顶角

	float fLBSunSateAngle; // 左下角太阳卫星相对方位角
	float fLTSunSateAngle; // 左上角太阳卫星相对方位角
	float fRBSunSateAngle; // 右下角太阳卫星相对方位角
	float fRTSunSateAngle; // 右上角太阳卫星相对方位角

	unsigned short wRevered[5]; //保留
	unsigned short wVersion; //版本号
	unsigned int dwSkipLength; //局地文件头记录的后面填充字段长度 = 
	//wWidth - sizeof(LocalFileHeader)
};


class LDF
{
public:
	LDF(void);
public:
	~LDF(void);
public:
	bool read(CString filepath);
	bool readHdr(CString filepath);
	int write(CString filepath);

public:
	LocalFileHeader m_hdr;
	int m_Sample;
	int m_Line;
	short ** m_Band;

public:
	//b 通道，1-5
	inline short getPixel(int b, int x, int y)
	{
		if(x >= 0 && x < m_Sample && y >= 0 && y < m_Line){
			return m_Band[b - 1][y * m_Sample + x];
		}
		else{
			return 0;
		}
	}

	inline bool setPixel(int b, int x, int y, short val)
	{
		if (x >= 0 && x < m_Sample && y >= 0 && y < m_Line){
			m_Band[b - 1][y * m_Sample + x] = val;
			return true;
		}
		else{
			return false;
		}
	}

	inline unsigned short getPixelGeo(int b, double x, double y)
	{
		int sx = 0, sy = 0;
		geo2pix(x, y, sx, sy);
		return getPixel(b, sx, sy);
	}

	inline unsigned short setPixelGeo(int b, double x, double y, unsigned short val)
	{
		int sx, sy;
		geo2pix(x, y, sx, sy);
		return setPixel(b, sx, sy, val);
	}

	inline void pix2geo(int x, int y, double& gx, double& gy)
	{
		gx = m_hdr.fMinLon + x * m_hdr.fLonSolution;
		gy = m_hdr.fMaxLat - y * m_hdr.fLatSolution;
	}
	
	inline void geo2pix(double gx, double gy, int& x, int& y)
	{
		x = int((gx - m_hdr.fMinLon) / m_hdr.fLonSolution + 0.5);
		y = int((m_hdr.fMaxLat - gy) / m_hdr.fLatSolution + 0.5);
		
	}
	double getWndMean(int b, double x, double y, int nSize);

};


int noCloudLdfBlend(LDF& ldf1, LDF& ldf2, int min, int max);
//LDF无云合成，合成后的数据存放在ldf1中。


double checkGeoExtent(LocalFileHeader hdr1, LocalFileHeader hdr2);
//检查两个LDF的地理范围重合程度。返回百分比。

void LDFSubtraction(LDF& ldf1, LDF& ldf2, short min, short max);
//ldf1-ldf2，结果放入ldf1, 不论ldf1, ldf2有无效值，则计该点为0;