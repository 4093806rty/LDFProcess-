#pragma once
#include<string>
#include<afx.h>
#include<vector>


struct LocalFileHeader
{
	char wFileID[2]; // �ֵ��ļ���ʶ,�̶�Ϊ�ַ���"LA" 
	unsigned short wSatelliteID; // ���Ǳ�ʶ, NOAA-14: 14, NOAA-12: 12
	unsigned short wOrbitNums; // �����
	unsigned short wUporDown; // ��������, 1: ����, 0: ����

	unsigned short wYear; // ��
	unsigned short wMonth; // ��
	unsigned short wDay; // ��
	unsigned short wHour; // ʱ
	unsigned short wMinute; // ��

	unsigned short wDayorNight; // �����ҹ��ʶ, 0: ����, 1: ��ҹ
	unsigned short wChannelNums; // ͨ����, ����: 5, ��ҹ: 3

	unsigned short wProjectType; // ͶӰ��ʽ, 0: ��ͶӰ, 1: �Ƚ�ͶӰ
	// 2: ����ͶӰ, 3: ������ͶӰ
	// 4: �������ͶӰ

	unsigned short wWidth; // ����
	unsigned short wHeight; // ����

	float fLonSolution; // ���ȷֱ��� -- �Ƚ�ͶӰ
	//��x�ֱ��� -- ���С������С��������ͶӰ 
	float fLatSolution; // γ�ȷֱ��� -- �Ƚ�ͶӰ
	//��y�ֱ��� -- ���С������С��������ͶӰ

	float fStandardLat1; //��׼γ��1--���С������С��������ͶӰ��Ч
	float fStandardLat2; //��׼γ��2 -- ������ͶӰ��Ч
	float fEarthR; // ����뾶--���С������С��������ͶӰ��Ч

	float fMinLat; // ��Сγ��
	float fMaxLat; // ���γ��
	float fMinLon; // ��С����
	float fMaxLon; // ��󾭶�

	float fLBSunAngle; // ���½�̫���춥��
	float fLTSunAngle; // ���Ͻ�̫���춥��
	float fRBSunAngle; // ���½�̫���춥��
	float fRTSunAngle; // ���Ͻ�̫���춥��

	float fLBSateAngle; // ���½������춥��
	float fLTSateAngle; // ���Ͻ������춥��
	float fRBSateAngle; // ���½������춥��
	float fRTSateAngle; // ���Ͻ������춥��

	float fLBSunSateAngle; // ���½�̫��������Է�λ��
	float fLTSunSateAngle; // ���Ͻ�̫��������Է�λ��
	float fRBSunSateAngle; // ���½�̫��������Է�λ��
	float fRTSunSateAngle; // ���Ͻ�̫��������Է�λ��

	unsigned short wRevered[5]; //����
	unsigned short wVersion; //�汾��
	unsigned int dwSkipLength; //�ֵ��ļ�ͷ��¼�ĺ�������ֶγ��� = 
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
	//b ͨ����1-5
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
//LDF���ƺϳɣ��ϳɺ�����ݴ����ldf1�С�


double checkGeoExtent(LocalFileHeader hdr1, LocalFileHeader hdr2);
//�������LDF�ĵ���Χ�غϳ̶ȡ����ذٷֱȡ�

void LDFSubtraction(LDF& ldf1, LDF& ldf2, short min, short max);
//ldf1-ldf2���������ldf1, ����ldf1, ldf2����Чֵ����Ƹõ�Ϊ0;