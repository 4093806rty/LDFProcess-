
#include "stdafx.h"
#include "Irmath.h"
#include<cmath>


double GetPointDistance(pointStruct p1, pointStruct p2)   
{  
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));  
}  


//判断线段AB上，与P3(以后用C表示)的最小距离
double GetNearestDistance(pointStruct PA, pointStruct PB, pointStruct P3)
{  
	//情况一：假设三个点构不成三角形，即至少有两个点重合
    //----------图2--------------------   
     double a, b, c;  
     a = GetPointDistance(PB, P3);  
     if(a <= 0.000001)  
      return 0.0;  
     b = GetPointDistance(PA, P3);  
     if(b <= 0.000001)  
      return 0.0;  
     c = GetPointDistance(PA, PB);  
     if(c <= 0.000001)  
      return a;    //如果PA和PB坐标相同，则退出函数，并返回距离   
    //------------------------------   
     

	//情况二：假设C在AB上的投影不在AB上 <=> ∠CAB或∠CBA为钝角
	//可以直接得出最小距离（∠CAB是钝角，则b<a；∠CBA是钝角，则a<b）

     if(a * a >= b * b + c * c)      //--------图3--------   
      return b;      //如果是钝角返回b   
     if(b * b >= a * a + c * c)      //--------图4-------   
      return a;      //如果是钝角返回a   
    
	//情况三：假设C在AB上的投影在AB上，即∠CAB与∠ABC都为锐角，那最小距离就是三角形的高了
    //图1   
     double l = (a + b + c) / 2;      //周长的一半   
     double s = sqrt(l * (l - a) * (l - b) * (l - c));  //海伦公式求面积，也可以用矢量求   
     return 2 * s / c;
}

//判断一条折线上，与Pa的最小距离
//思想为把折线分成小线段来处理，求出每个小线段与Pa的最小距离，然后得到这些距离的最小值
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

//(x1,y1)与(x2,y2)之间的距离
double distance(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


//统计均值、方差、标准差、最大、最小值
statis stat(std::vector<short>data)
{
	statis st;
	st.avg = 0;//均值
	st.dev = 0;//标准差
	st.max = 0;
	st.min = 0;
	st.var = 0;//方差
	if(data.size() == 0){
		return st;
	}

	st.max = data[0] / 10.0;//由于ldf存储的是放大10倍的值
	st.min = data[0] / 10.0;

	double sum = 0;
	for(size_t i = 0; i < data.size(); i++){
		double v = data[i] / 10.0;
		sum += v;
		if(st.min > v){
			st.min = v;//data中的最小值
		}
		if(st.max < v){
			st.max = v;//data中的最大值
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
		q.x = rand() + offset;//随机取一个足够远的点q  
		q.y = rand() + offset;//以p为起点q为终点做射线L  
		for (counter = i = 0; i<len; i++){//依次对多边形的每条边进行考察  
			if (fabs(cross(p, arr[i], arr[(i + 1) % len]))<eps &&
				(arr[i].x - p.x)*(arr[(i + 1) % len].x - p.x)<eps &&
				(arr[i].y - p.y)*(arr[(i + 1) % len].y - p.y)<eps)
				return on_edge; //点p在边上,返回on_edge  
			else if (fabs(cross(p, q, arr[i]))<eps)
				break; //点arr[i]在射线pq上，停止本循环，另取q  
			else if (cross(p, arr[i], q)*cross(p, arr[(i + 1) % len], q)<-eps &&
				cross(arr[i], p, arr[(i + 1) % len])*cross(arr[i], q, arr[(i + 1) % len])<-eps)
				counter++;
		}
	}
	return counter & 1;
}*/

bool PointRgn(double x, double y, int Number, pointStruct *pt)
//下面采用射线法进行判断点和多边形的关系
{ /*射线法的思想是对点任意引出一条射线，
  如果该直线和多边形的交点为奇数时，
  在多边形内 否则在多边形外*/ 
	//本算法中采用向右水平射线
	//注意：：算法中没有通过求交点的方法来求出点的个数，
	//而是采用二分法逐一进行判断
	int count = 0;
	for(int i = 0;i < Number - 1; i++)
	{	
		pointStruct pt1,pt2; //定义两个暂时点来保存各个线段的端点
		if(pt[i].y <= pt[i+1].y) //将上边的点放在pt2中
		{
			pt1 = pt[i];
			pt2 = pt[i+1];
		}
		else 
		{ 
			pt1 = pt[i+1];
			pt2 = pt[i];
		}
		if(pt[i].x< x && pt[i+1].x < x) //线段的两个点都在但左侧 
			continue;
		else if(pt[i].y >= y && pt[i+1].y > y) //线段的两个端点都在上侧
			continue;
		else if(pt[i].y< y && pt[i+1].y < y) //线段的两个端点都在下侧
			continue;
		else if(pt[i].x > x && pt[i+1].x > x) //以上条件都不满足，同时线段在该点的右侧一定存在交点
			count++;
		else
		{ 
			while(true)
			{ 
				pointStruct M;//二分法的中点 
				M.x = (pt1.x + pt2.x) / 2;
				M.y = (pt1.y + pt2.y) / 2;
				if(M.y > y)
					pt2 = M;
				else
					pt1 = M;
				if(pt2.x > pt1.x)// 如果pt2在pt1的右侧
				{ 
					if(pt2.x < x)
						break;
					if(pt1.x > x)
					{ 
						count++;
						break;
					}
				} 
				else //左侧 
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

//为气温台站数据写的函数
double getStationDayTemperature(std::vector<stationT> vecST, int year, int month, int day)
{
	stationT st;
	double sum = 0;
	int cnt = 0;
	for(size_t i = 0; i < vecST.size(); i++){
		st = vecST[i];
		if(st.year == year && st.month == month && st.day == day){
			if(st.hour >=1 && st.hour >=3 && st.temperature < 999 && st.temperature > -273.15){// 是不是想计算凌晨1点至3点的数据，那应该该改成st.hour <=3
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

//为气温台站数据写的函数
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

//将统计区域的有效值存到std::vector<short>ValTemp中，再交由stat做统计（圆形区域）
statis statAvg(LDF& ldf1, double CenterLon, double CenterLat, double R, int nBand, double min, double max)
{
	/*有可能是想得到stat(ValTemp)的值，然后再return st
	不知道max和min是不是故意搞反的，已无从查证
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

//将统计区域的有效值存到std::vector<short>ValTemp中，再交由stat做统计（矩形区域）
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