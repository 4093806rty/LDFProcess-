#pragma once
#include "afxwin.h"
#include "LDFToPicture.h"
/**********************************ColorButton��չ���1********************************************/
//���ColorButton.cpp��ColorButton.h��ColourPopup.cpp��ColourPopup.h������
#include "ColorButton.h"
#include "afxcmn.h"
/**************************************************************************************************/


// CLegendDlg �Ի���

class CLegendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLegendDlg)

public:
	CLegendDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLegendDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LEGEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


/**********************************ColorButton��չ���4********************************************/
    afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnSelChange(UINT lParam, LONG wParam);
    afx_msg LONG OnCloseUp(UINT lParam, LONG wParam);
    afx_msg LONG OnDropDown(UINT lParam, LONG wParam);
/**************************************************************************************************/


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSelchangeListFileLegend();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedApplyLegend();
	BOOL draw_picture_showlegend();

public:
	double m_dmin;
	double m_dmax;
	double m_dkmin;
	double m_dkmax;
	int m_fontsize_legend;
	CListBox m_listFile_legend;
	LDFToPicture m_pLPic;

	/*Point2D m_origin;		// �Ӵ�ԭ�㣨�Ӵ����Ͻǵľ�γ�ȣ�
	double m_ratioX;		// ����ϵ��
	double m_ratioY;*/
	double m_rate;
	double m_rate_origin;
	IProvider * m_pProvider_legend;
	int m_Channel;//������ͨ��
	double m_rate_Zoom;//��ʾͼ��ʱ�Ŵ���С

	/*void DrawMap(CDC* pDC, IProvider * pProvider);
	void InitMap(IProvider * pProvider);
	void GeoToMap(const shp::Point * geoPts, POINT * pts, int count);
	void GeoToMap(double geoX, double geoY, POINT * pt);
	void MapToGeo(int x, int y, Point2D * geoPoint);*/
	BOOL OpenShape(LPCTSTR lpszPathName);

	BOOL m_CHECK_epicenter;
	BOOL m_CHECK_provin_border;
	BOOL m_CHECK_fault;
	afx_msg void OnBnClickedCheckepicenter();
	afx_msg void OnBnClickedCheckprovinborder();
	afx_msg void OnBnClickedCheckfault();


	CComboBox m_COMO_SelChannel;//��Ͽ�ѡ��ͨ��
	afx_msg void OnSelchangeComboSelchannel();
	afx_msg void OnBnClickedBtnZoomin();
	afx_msg void OnBnClickedBtnZoomout();
	afx_msg void OnBnClickedBtnSave();

/**********************************ColorButton��չ���3********************************************/
	//�ڶ����Ǵ���Button���޸���ID
	/*********************************************************************
	�Ժ������ColorButton����Ҫ�Ĳ����ǣ�2��3��5��7
	2���Ի����ϴ�����ͨButton���޸���ID
	3������ؼ�����CColorButton��COLORREF��������ã����Բ��ã�
	5���󶨿ؼ�ID�Ϳؼ�����m_ClrBtn_****��COLORREFCOLORREF��������ã����Բ��ã�
	7��ÿ���ؼ���ʼ����Ĭ������
	//8��OnSelEndOK�б�д������ɫ��Ĵ��루��ʵ����ʹ�õĵط������磺����������������
	***********************************************************************/
	
	CColorButton m_ClrBtn_epicenter;
	CColorButton m_ClrBtn_provinborder;
	CColorButton m_ClrBtn_fault;
	CColorButton m_ClrBtn_StasticArea;//ͳ�������ɰ�ı����ɫ,͸������m_Slider_StatisticArea_ColorTrans����
	//COLORREF ThrowAwayColor;//CColorButton�൱��Control������COLORREF�൱��value������������OnSelEndOKд����update֮���
/**************************************************************************************************/


	int m_size_epicenter;
	int m_size_provinborder;
	int m_size_fault;
	CString m_Shape_strInfo;//shpͼ����ʾ���ԣ���ɫ���߿�
	CListCtrl m_List_StatisticArea;//��CString m_StasticArea_strInfo����;//��Ÿ���ͳ������γ�ȡ���Χ
	int m_StasticArea_SelectStyle;//ͳ���������ʽ��Բ��or����or���ѣ�
	//m_ClrBtn_StasticArea��m_Slider_StatisticArea_ColorTrans �� COLORREF m_StasticArea_Color����;
	CSliderCtrl m_Slider_StatisticArea_ColorTrans;//ͳ�������ɰ�ı����ɫ��͸����,��ɫ��m_ClrBtn_StasticArea����


	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnCustomdrawSliderTransStatisticarea(NMHDR *pNMHDR, LRESULT *pResult);

};
