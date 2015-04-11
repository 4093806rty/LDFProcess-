
#pragma once
#include<vector>
#include"LDF.h"


struct pointStruct
{
	double x;
	double y;
};

struct statis{
	double avg; //��ֵ
	double max; //���ֵ
	double min; //��Сֵ
	double dev; //��׼��
	double var; //����
};

struct stationT{
	int year;
	int month;
	int day;
	int hour;
	double temperature;
};


double GetPointDistance(pointStruct p1, pointStruct p2);
double GetNearestDistance(pointStruct PA, pointStruct PB, pointStruct P3);
double getNearestDistance(std::vector<pointStruct>&pts, pointStruct Pa);
statis stat(std::vector<short>data);
double getStationDayTemperature(std::vector<stationT> vecST, int year, int month, int day);
void getLDFdate(LDF& ldf, int& year, int& month, int& day);
statis statAvg(LDF& ldf1, double CenterLon, double CenterLat, double R, int nBand, double min, double max);
//����ָ������Χ�����¾�ֵ��CenterLon, CenterLat, R��λΪ��, 
//max, min Ϊ���·�Χ


statis statAvg(LDF& ldf1, double minLon, double maxLon,
	double minLat, double maxLat, int nBand, double min, double max);
//����ָ������Χ()�����¾�ֵ��, 
//max, min Ϊ���·�Χ

