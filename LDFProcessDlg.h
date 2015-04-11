// LDFProcessDlg.h : header file
//

#if !defined(AFX_LDFPROCESSDLG_H__C9EC633F_D806_484F_BC92_92E95DC0A181__INCLUDED_)
#define AFX_LDFPROCESSDLG_H__C9EC633F_D806_484F_BC92_92E95DC0A181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\Lib\LDF.h"
#include "LDFToPicture.h"
#include ".\Lib\IProvider.h"
#include "processUIThread.h"
#include ".\Lib\Irmath.h"
using namespace UltraGIS;

UINT ThreadFunc(LPVOID pParm);//线程函数的定义：文件复制线程

/////////////////////////////////////////////////////////////////////////////
// CLDFProcessDlg dialog

class CLDFProcessDlg : public CDialog
{
// Construction
public:
	//CString generateOutFilename(CString, CString);
	CLDFProcessDlg(CWnd* pParent = NULL);	// standard constructor
	~CLDFProcessDlg();
	int m_selectinputstyle;
	CString m_inputfolder;
	BOOL m_InputWindow_Extend;
	CStringArray m_strArray;
	double m_thrld_Month[13];
	int m_algorithm;
	BOOL m_CHEK_Clddetection2;
	BOOL m_CHEK_Pseudocolor;
	BOOL m_CHEK_Kpseudocolor;
	BOOL m_CHEK_Statistics;
	BOOL m_CHEK_Compound;
	LDF m_ldf;
	LDFToPicture m_pLPic;
	int m_selectoutputstyle;
	CString m_outputfolder;
	CString m_bkgndfile;

	//用以保存CLegendDlg中的设置
	double m_dmin;//出图显示范围，超限则设置为最大最小值
	double m_dmax;
	double m_dkmin;
	double m_dkmax;
	int m_fontsize_legend;
	BOOL m_CHECK_epicenter;
	BOOL m_CHECK_provin_border;
	BOOL m_CHECK_fault;
	int m_Channel;//待计算通道
	CString m_Shape_strInfo;//shp图的显示属性（颜色、线宽）
	CString m_StasticArea_strInfo;//存放各个统计区域经纬度、范围
	int m_StasticArea_SelectStyle;//统计区域的样式（圆形or矩形or断裂）
	COLORREF m_StasticArea_Color;//统计区域蒙板的标记颜色（包括透明度）


	CWinThread *pUIThread;//界面线程，用于进度的显示
	CWinThread *pThread;  //工作线程，用于文件的复制
	//unsigned long m_nSpeed;
	float    m_fTime;

	Point2D m_origin;		// 视窗原点（视窗左上角的经纬度）
	double m_ratioX;			// 缩放系数
	double m_ratioY;
	double m_rate;
	IProvider * m_pProvider;


	/*void DrawMap(CDC* pDC, IProvider * pProvider);
	void InitMap(IProvider * pProvider);
	void GeoToMap(const shp::Point * geoPts, POINT * pts, int count);
	void GeoToMap(double geoX, double geoY, POINT * pt);
	void MapToGeo(int x, int y, Point2D * geoPoint);*/
	BOOL OpenShape(LPCTSTR lpszPathName);


// Dialog Data
	//{{AFX_DATA(CLDFProcessDlg)
	enum { IDD = IDD_LDFPROCESS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLDFProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLDFProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBUTBrowser();
	afx_msg void OnBUTInput();
	afx_msg void OnBUTCLOUDetection();
	afx_msg void OnBUTAbnormal();
	afx_msg void OnBUTOutput();
	afx_msg void OnButCalculate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	/*****************************统计*********************************/
	//通道、最小、最大值自动获取，不再传参数，分别对应：m_Channel、m_thrld_Month[ldf1.m_hdr.wMonth]、350
	//m_StasticArea_SelectStyle分为：方法0、方法1、方法2
	//方法0：圆形统计区域
	void statCircle(double m_dCenterLon,double m_dCenterLat,double m_dRadius);
	//方法1：矩形统计区域
	void statRect(double m_dCenterLon,double m_dCenterLat,double m_dRadius_Width,double m_dRadius_Height);
	//矢量转点
	void PolyLineToPoints(IProvider * pProvider, std::vector<int>& cts, std::vector<pointStruct>& points);
	//对单幅影像统计断裂带内外结果
	statis statSinglePolyLine(LDF& ldf, std::vector<pointStruct>&pts, double minDis, double maxDis,
		int nBand, double min, double max, statis& baseST);
	//方法2：断裂带统计区域，对每幅影像，调用一次statSinglePolyLine
	void statPolyLine(std::vector<int>& cts, std::vector<pointStruct>& points, double minDis, double maxDis,
		int nBand, double min, double max);
	/********************************************************************/

	CString GetAppPath();//得到程序全路径（以存放config.ini、或读取shp）
	void GetConSetting();
	void SetConSetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedButLegend();
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDFPROCESSDLG_H__C9EC633F_D806_484F_BC92_92E95DC0A181__INCLUDED_)
