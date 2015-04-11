
#pragma once
#include<vector>
#include"LDF.h"


struct pointStruct
{
	double x;
	double y;
};

struct statis{
	double avg; //均值
	double max; //最大值
	double min; //最小值
	double dev; //标准差
	double var; //方差
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
//计算指定地理范围的亮温均值，CenterLon, CenterLat, R单位为度, 
//max, min 为亮温范围


statis statAvg(LDF& ldf1, double minLon, double maxLon,
	double minLat, double maxLat, int nBand, double min, double max);
//计算指定地理范围()的亮温均值，, 
//max, min 为亮温范围

