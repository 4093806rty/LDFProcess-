#pragma once
#include "afxwin.h"
#include "LDFToPicture.h"
/**********************************ColorButton扩展添加1********************************************/
//添加ColorButton.cpp、ColorButton.h、ColourPopup.cpp、ColourPopup.h到工程
#include "ColorButton.h"
#include "afxcmn.h"
/**************************************************************************************************/


// CLegendDlg 对话框

class CLegendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLegendDlg)

public:
	CLegendDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLegendDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LEGEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


/**********************************ColorButton扩展添加4********************************************/
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

	/*Point2D m_origin;		// 视窗原点（视窗左上角的经纬度）
	double m_ratioX;		// 缩放系数
	double m_ratioY;*/
	double m_rate;
	double m_rate_origin;
	IProvider * m_pProvider_legend;
	int m_Channel;//待计算通道
	double m_rate_Zoom;//显示图像时放大缩小

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


	CComboBox m_COMO_SelChannel;//组合框选择通道
	afx_msg void OnSelchangeComboSelchannel();
	afx_msg void OnBnClickedBtnZoomin();
	afx_msg void OnBnClickedBtnZoomout();
	afx_msg void OnBnClickedBtnSave();

/**********************************ColorButton扩展添加3********************************************/
	//第二步是创建Button，修改其ID
	/*********************************************************************
	以后添加再ColorButton还需要的操作是：2、3、5、7
	2、对话框上创建普通Button，修改其ID
	3、定义控件变量CColorButton（COLORREF不清楚作用，可以不用）
	5、绑定控件ID和控件变量m_ClrBtn_****（COLORREFCOLORREF不清楚作用，可以不用）
	7、每个控件初始化的默认设置
	//8、OnSelEndOK中编写设置颜色后的代码（其实就是使用的地方，例如：交互，不再详述）
	***********************************************************************/
	
	CColorButton m_ClrBtn_epicenter;
	CColorButton m_ClrBtn_provinborder;
	CColorButton m_ClrBtn_fault;
	CColorButton m_ClrBtn_StasticArea;//统计区域蒙板的标记颜色,透明度由m_Slider_StatisticArea_ColorTrans控制
	//COLORREF ThrowAwayColor;//CColorButton相当于Control变量，COLORREF相当于value变量，还得在OnSelEndOK写代码update之类的
/**************************************************************************************************/


	int m_size_epicenter;
	int m_size_provinborder;
	int m_size_fault;
	CString m_Shape_strInfo;//shp图的显示属性（颜色、线宽）
	CListCtrl m_List_StatisticArea;//与CString m_StasticArea_strInfo交互;//存放各个统计区域经纬度、范围
	int m_StasticArea_SelectStyle;//统计区域的样式（圆形or矩形or断裂）
	//m_ClrBtn_StasticArea、m_Slider_StatisticArea_ColorTrans 与 COLORREF m_StasticArea_Color交互;
	CSliderCtrl m_Slider_StatisticArea_ColorTrans;//统计区域蒙板的标记颜色的透明度,颜色由m_ClrBtn_StasticArea控制


	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnCustomdrawSliderTransStatisticarea(NMHDR *pNMHDR, LRESULT *pResult);

};
