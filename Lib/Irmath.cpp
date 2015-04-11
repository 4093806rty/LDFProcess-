
#include "stdafx.h"
#include "Irmath.h"
#include<cmath>


double GetPointDistance(pointStruct p1, pointStruct p2)   
{  
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));  
}  


//�ж��߶�AB�ϣ���P3(�Ժ���C��ʾ)����С����
double GetNearestDistance(pointStruct PA, pointStruct PB, pointStruct P3)
{  
	//���һ�����������㹹���������Σ����������������غ�
    //----------ͼ2--------------------   
     double a, b, c;  
     a = GetPointDistance(PB, P3);  
     if(a <= 0.000001)  
      return 0.0;  
     b = GetPointDistance(PA, P3);  
     if(b <= 0.000001)  
      return 0.0;  
     c = GetPointDistance(PA, PB);  
     if(c <= 0.000001)  
      return a;    //���PA��PB������ͬ�����˳������������ؾ���   
    //------------------------------   
     

	//�����������C��AB�ϵ�ͶӰ����AB�� <=> ��CAB���CBAΪ�۽�
	//����ֱ�ӵó���С���루��CAB�Ƕ۽ǣ���b<a����CBA�Ƕ۽ǣ���a<b��

     if(a * a >= b * b + c * c)      //--------ͼ3--------   
      return b;      //����Ƕ۽Ƿ���b   
     if(b * b >= a * a + c * c)      //--------ͼ4-------   
      return a;      //����Ƕ۽Ƿ���a   
    
	//�����������C��AB�ϵ�ͶӰ��AB�ϣ�����CAB���ABC��Ϊ��ǣ�����С������������εĸ���
    //ͼ1   
     double l = (a + b + c) / 2;      //�ܳ���һ��   
     double s = sqrt(l * (l - a) * (l - b) * (l - c));  //���׹�ʽ�������Ҳ������ʸ����   
     return 2 * s / c;
}

//�ж�һ�������ϣ���Pa����С����
//˼��Ϊ�����߷ֳ�С�߶����������ÿ��С�߶���Pa����С���룬Ȼ��õ���Щ�������Сֵ
double getNearestDistance(std::vector<pointStruct>&pts, pointStruct Pa)
{
	std::vector<double>vecdis;

	for(size_t i = 0; i < pts.size() - 1; i++){
		double dis = GetNearestDistance(pts[i], pts[i+1], Pa);
		vecdis.push_back(dis);
	}

	double min = vecdis[0];
	for(size_t i = 0; i < vecdis.size(); i++){
		if(min > vecdis[i]){
			min = vecdis[i];
		}
	}
	return min;
}

//(x1,y1)��(x2,y2)֮��ľ���
double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


//ͳ�ƾ�ֵ�������׼������Сֵ
statis stat(std::vector<short>data)
{
	statis st;
	st.avg = 0;//��ֵ
	st.dev = 0;//��׼��
	st.max = 0;
	st.min = 0;
	st.var = 0;//����
	if(data.size() == 0){
		return st;
	}

	st.max = data[0] / 10.0;//����ldf�洢���ǷŴ�10����ֵ
	st.min = data[0] / 10.0;

	double sum = 0;
	for(size_t i = 0; i < data.size(); i++){
		double v = data[i] / 10.0;
		sum += v;
		if(st.min > v){
			st.min = v;//data�е���Сֵ
		}
		if(st.max < v){
			st.max = v;//data�е����ֵ
		}
	}
	st.avg = sum / data.size();
	sum = 0;
	for(size_t i = 0; i < data.size(); i++){
		double v = data[i] / 10.0;
		sum += (v -st.avg) * (v -st.avg);
	}
	st.var = sum / data.size();
	st.dev = sqrt(st.var);
	return st;
}


/*
const int offset = 1000;
const double eps = 1e-8;


double cross(pointStruct pi, pointStruct pj, pointStruct pk){ // (pi,pj)X(pi,pk)  
	return (pj.x - pi.x)*(pk.y - pi.y) - (pj.y - pi.y)*(pk.x - pi.x);
}

int InPolygon(const pointStruct *arr, const int &len, const pointStruct &p, int on_edge = 1){
	pointStruct q;
	int i = 0, counter;
	while (i<len){
		q.x = rand() + offset;//���ȡһ���㹻Զ�ĵ�q  
		q.y = rand() + offset;//��pΪ���qΪ�յ�������L  
		for (counter = i = 0; i<len; i++){//���ζԶ���ε�ÿ���߽��п���  
			if (fabs(cross(p, arr[i], arr[(i + 1) % len]))<eps &&
				(arr[i].x - p.x)*(arr[(i + 1) % len].x - p.x)<eps &&
				(arr[i].y - p.y)*(arr[(i + 1) % len].y - p.y)<eps)
				return on_edge; //��p�ڱ���,����on_edge  
			else if (fabs(cross(p, q, arr[i]))<eps)
				break; //��arr[i]������pq�ϣ�ֹͣ��ѭ������ȡq  
			else if (cross(p, arr[i], q)*cross(p, arr[(i + 1) % len], q)<-eps &&
				cross(arr[i], p, arr[(i + 1) % len])*cross(arr[i], q, arr[(i + 1) % len])<-eps)
				counter++;
		}
	}
	return counter & 1;
}*/

bool PointRgn(double x, double y, int Number, pointStruct *pt)
//����������߷������жϵ�Ͷ���εĹ�ϵ
{ /*���߷���˼���ǶԵ���������һ�����ߣ�
  �����ֱ�ߺͶ���εĽ���Ϊ����ʱ��
  �ڶ������ �����ڶ������*/ 
	//���㷨�в�������ˮƽ����
	//ע�⣺���㷨��û��ͨ���󽻵�ķ����������ĸ�����
	//���ǲ��ö��ַ���һ�����ж�
	int count = 0;
	for(int i = 0;i < Number - 1; i++)
	{	
		pointStruct pt1,pt2; //����������ʱ������������߶εĶ˵�
		if(pt[i].y <= pt[i+1].y) //���ϱߵĵ����pt2��
		{
			pt1 = pt[i];
			pt2 = pt[i+1];
		}
		else 
		{ 
			pt1 = pt[i+1];
			pt2 = pt[i];
		}
		if(pt[i].x< x && pt[i+1].x < x) //�߶ε������㶼�ڵ���� 
			continue;
		else if(pt[i].y >= y && pt[i+1].y > y) //�߶ε������˵㶼���ϲ�
			continue;
		else if(pt[i].y< y && pt[i+1].y < y) //�߶ε������˵㶼���²�
			continue;
		else if(pt[i].x > x && pt[i+1].x > x) //���������������㣬ͬʱ�߶��ڸõ���Ҳ�һ�����ڽ���
			count++;
		else
		{ 
			while(true)
			{ 
				pointStruct M;//���ַ����е� 
				M.x = (pt1.x + pt2.x) / 2;
				M.y = (pt1.y + pt2.y) / 2;
				if(M.y > y)
					pt2 = M;
				else
					pt1 = M;
				if(pt2.x > pt1.x)// ���pt2��pt1���Ҳ�
				{ 
					if(pt2.x < x)
						break;
					if(pt1.x > x)
					{ 
						count++;
						break;
					}
				} 
				else //��� 
				{ 
					if(pt1.x < x)
						break;
					if(pt2.x > x)
					{ 
						count++;
						break;
					} 
				} 
			} 
		} 
	} 
	if(count % 2 == 0)
		return false;
	else
		return true;
}

//Ϊ����̨վ����д�ĺ���
double getStationDayTemperature(std::vector<stationT> vecST, int year, int month, int day)
{
	stationT st;
	double sum = 0;
	int cnt = 0;
	for(size_t i = 0; i < vecST.size(); i++){
		st = vecST[i];
		if(st.year == year && st.month == month && st.day == day){
			if(st.hour >=1 && st.hour >=3 && st.temperature < 999 && st.temperature > -273.15){// �ǲ���������賿1����3������ݣ���Ӧ�øøĳ�st.hour <=3
				cnt++;
				sum += st.temperature;
			}
		}
	}
	if(cnt > 0)
		return sum / cnt + 273.15;
	else
		return 999;
}

//Ϊ����̨վ����д�ĺ���
void getLDFdate(LDF& ldf, int& year, int& month, int& day)
{
	year = ldf.m_hdr.wYear;
	month = ldf.m_hdr.wMonth;
	day = ldf.m_hdr.wDay;
	if(year > 2100){
		year = year % 100;
		year += 2000;
	}
	if(year < 100 && year > 70){
		year += 1900;
	}
	if(year < 70){
		year += 2000;
	}
}

//��ͳ���������Чֵ�浽std::vector<short>ValTemp�У��ٽ���stat��ͳ�ƣ�Բ������
statis statAvg(LDF& ldf1, double CenterLon, double CenterLat, double R, int nBand, double min, double max)
{
	/*�п�������õ�stat(ValTemp)��ֵ��Ȼ����return st
	��֪��max��min�ǲ��ǹ���㷴�ģ����޴Ӳ�֤
	statis st;
	st.max = min;
	st.min = max;*/
	size_t sample = ldf1.m_hdr.wWidth;
	size_t line = ldf1.m_hdr.wHeight;

	std::vector<short>ValTemp;
	for(size_t iy = 0; iy < line; iy++){
		for(size_t ix = 0; ix < sample; ix++){
			double gx = 0, gy = 0;
			ldf1.pix2geo(ix, iy, gx, gy);
			double dis = distance(gx, gy, CenterLon, CenterLat);
			if(dis <= R){
				short val = ldf1.getPixel(nBand, ix, iy);
				if(val <= max * 10.0 && val >= min * 10.0){
					ValTemp.push_back(val);
				}
			}
		}
	}
	return stat(ValTemp);
}

//��ͳ���������Чֵ�浽std::vector<short>ValTemp�У��ٽ���stat��ͳ�ƣ���������
statis statAvg(LDF& ldf1, double minLon, double maxLon,
	double minLat, double maxLat, int nBand, double min, double max)
{
	/*statis st;
	st.max = min;
	st.min = max;*/
	size_t sample = ldf1.m_hdr.wWidth;
	size_t line = ldf1.m_hdr.wHeight;

	long cnt = 0;
	double sum = 0;
	std::vector<short>ValTemp;
	for(size_t iy = 0; iy < line; iy++){
		for(size_t ix = 0; ix < sample; ix++){
			double gx = 0, gy = 0;
			ldf1.pix2geo(ix, iy, gx, gy);
			if(gx >= minLon && gx <= maxLon && gy >= minLat && gy <= maxLat){
				short val = ldf1.getPixel(nBand, ix, iy);
				if(val <= max * 10.0 && val >= min * 10.0){
					ValTemp.push_back(val);
				}
			}
		}
	}
	return stat(ValTemp);
}