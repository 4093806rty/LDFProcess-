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

UINT ThreadFunc(LPVOID pParm);//�̺߳����Ķ��壺�ļ������߳�

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

	//���Ա���CLegendDlg�е�����
	double m_dmin;//��ͼ��ʾ��Χ������������Ϊ�����Сֵ
	double m_dmax;
	double m_dkmin;
	double m_dkmax;
	int m_fontsize_legend;
	BOOL m_CHECK_epicenter;
	BOOL m_CHECK_provin_border;
	BOOL m_CHECK_fault;
	int m_Channel;//������ͨ��
	CString m_Shape_strInfo;//shpͼ����ʾ���ԣ���ɫ���߿�
	CString m_StasticArea_strInfo;//��Ÿ���ͳ������γ�ȡ���Χ
	int m_StasticArea_SelectStyle;//ͳ���������ʽ��Բ��or����or���ѣ�
	COLORREF m_StasticArea_Color;//ͳ�������ɰ�ı����ɫ������͸���ȣ�


	CWinThread *pUIThread;//�����̣߳����ڽ��ȵ���ʾ
	CWinThread *pThread;  //�����̣߳������ļ��ĸ���
	//unsigned long m_nSpeed;
	float    m_fTime;

	Point2D m_origin;		// �Ӵ�ԭ�㣨�Ӵ����Ͻǵľ�γ�ȣ�
	double m_ratioX;			// ����ϵ��
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
	/*****************************ͳ��*********************************/
	//ͨ������С�����ֵ�Զ���ȡ�����ٴ��������ֱ��Ӧ��m_Channel��m_thrld_Month[ldf1.m_hdr.wMonth]��350
	//m_StasticArea_SelectStyle��Ϊ������0������1������2
	//����0��Բ��ͳ������
	void statCircle(double m_dCenterLon,double m_dCenterLat,double m_dRadius);
	//����1������ͳ������
	void statRect(double m_dCenterLon,double m_dCenterLat,double m_dRadius_Width,double m_dRadius_Height);
	//ʸ��ת��
	void PolyLineToPoints(IProvider * pProvider, std::vector<int>& cts, std::vector<pointStruct>& points);
	//�Ե���Ӱ��ͳ�ƶ��Ѵ�������
	statis statSinglePolyLine(LDF& ldf, std::vector<pointStruct>&pts, double minDis, double maxDis,
		int nBand, double min, double max, statis& baseST);
	//����2�����Ѵ�ͳ�����򣬶�ÿ��Ӱ�񣬵���һ��statSinglePolyLine
	void statPolyLine(std::vector<int>& cts, std::vector<pointStruct>& points, double minDis, double maxDis,
		int nBand, double min, double max);
	/********************************************************************/

	CString GetAppPath();//�õ�����ȫ·�����Դ��config.ini�����ȡshp��
	void GetConSetting();
	void SetConSetting();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClickedButLegend();
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LDFPROCESSDLG_H__C9EC633F_D806_484F_BC92_92E95DC0A181__INCLUDED_)
