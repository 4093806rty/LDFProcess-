// LegendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "LegendDlg.h"
#include "afxdialogex.h"
#include "LDFProcessDlg.h"
#include ".\lib\ShapeProvider.h"


// CLegendDlg 对话框

IMPLEMENT_DYNAMIC(CLegendDlg, CDialogEx)

CLegendDlg::CLegendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLegendDlg::IDD, pParent)
	, m_dmin(0)
	, m_dmax(0)
	, m_dkmin(0)
	, m_dkmax(0)
	, m_fontsize_legend(0)
	, m_CHECK_epicenter(FALSE)
	, m_CHECK_provin_border(FALSE)
	, m_CHECK_fault(FALSE)
	, m_Channel(0)
	, m_rate_Zoom(1.0)
	, m_rate_origin(-1.0)//注意这个千万不要修改，后面会有判断是不是第一次初始化
	, m_Shape_strInfo(_T(""))
	//, m_StasticArea_strInfo(0)//注意，m_List_StatisticArea与LDFProcess 的 m_StasticArea_strInfo交互，因此LegendDlg中不存在该变量
	, m_StasticArea_SelectStyle(0)
	, m_size_epicenter(0)
	, m_size_provinborder(0)
	, m_size_fault(0)
{
	m_pProvider_legend=NULL;
}

CLegendDlg::~CLegendDlg()
{
	if (m_pProvider_legend)
	{
		delete m_pProvider_legend;
		m_pProvider_legend = NULL;
	}
}


/*	DDX_TEXT()		的作用可以理解为把字符串变量和控件的文本（WindowText）关联起来，
	DDX_Control()	的作用可以理解为把变量和控件本身关联起来，
	DoDataExchange(pDX)	就是处理所有变量与其关联控件交换数据的函数。
	在程序中
	DDX_Text(pDX, IDC_ID_VALUE, m_strID);
	DDX_Control(pDX, IDC_ID_VALUE, m_editID);//通过修改变量的数值控制控件操作(两者相互控制)
	通过update(TRUE)取得控件上的值到m_strID，处理修改后通过update(FALSE)传回控件，界面显示
	通过m_editID.GetWindText(str),取得值，处理后，通过m_editID.SetWindText(str)传回控件界面显示
	将ID为IDC_ID_VALUEO的控件的文字与变量m_strID进行绑定。
	调用UpdateData（TRUE）时，将控件的文字赋值给变量
	调用UpdateData（FALSE）时，将变量的值赋值给控件的文字
*/

void CLegendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MINVALUE, m_dmin);
	DDX_Text(pDX, IDC_EDIT_MAXVALUE, m_dmax);
	DDX_Text(pDX, IDC_EDIT_KMINVALUE, m_dkmin);
	DDX_Text(pDX, IDC_EDIT_KMAXVALUE, m_dkmax);
	DDX_Text(pDX, IDC_EDIT_FONTSIZE_LON_LAT, m_fontsize_legend);
	DDX_Control(pDX, IDC_LIST_FILE_LEGEND, m_listFile_legend);

	DDX_Check(pDX, IDC_CHECK_epicenter, m_CHECK_epicenter);
	DDX_Check(pDX, IDC_CHECK_provin_border, m_CHECK_provin_border);
	DDX_Check(pDX, IDC_CHECK_fault, m_CHECK_fault);
	DDX_Control(pDX, IDC_COMBO_SelChannel, m_COMO_SelChannel);
	/**********************************ColorButton扩展添加5********************************************/
	DDX_Control(pDX, IDC_ClrBtn_epicenter, m_ClrBtn_epicenter);
	DDX_Control(pDX, IDC_ClrBtn_provinborder, m_ClrBtn_provinborder);
	DDX_Control(pDX, IDC_ClrBtn_fault, m_ClrBtn_fault);
	DDX_Control(pDX, IDC_ClrBtn_StatisticArea, m_ClrBtn_StasticArea);

	//DDX_ColorButton(pDX, IDC_ClrBtn_epicenter, ThrowAwayColor);
	/**************************************************************************************************/
	DDX_Control(pDX, IDC_LIST_StatisticArea, m_List_StatisticArea);
	DDX_Radio(pDX, IDC_RADIO_circle, m_StasticArea_SelectStyle);
	DDX_Text(pDX, IDC_EDIT_SIZE_epicenter, m_size_epicenter);
	DDX_Text(pDX, IDC_EDIT_SIZE_provinborder, m_size_provinborder);
	DDX_Text(pDX, IDC_EDIT_SIZE_fault, m_size_fault);
	DDX_Control(pDX, IDC_SLIDER_Trans_StatisticArea, m_Slider_StatisticArea_ColorTrans);
}


BEGIN_MESSAGE_MAP(CLegendDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLegendDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_FILE_LEGEND, &CLegendDlg::OnSelchangeListFileLegend)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_APPLY_LEGEND, &CLegendDlg::OnBnClickedApplyLegend)
	ON_BN_CLICKED(IDC_CHECK_epicenter, &CLegendDlg::OnBnClickedCheckepicenter)
	ON_BN_CLICKED(IDC_CHECK_provin_border, &CLegendDlg::OnBnClickedCheckprovinborder)
	ON_BN_CLICKED(IDC_CHECK_fault, &CLegendDlg::OnBnClickedCheckfault)
	ON_CBN_SELCHANGE(IDC_COMBO_SelChannel, &CLegendDlg::OnSelchangeComboSelchannel)
	ON_BN_CLICKED(ID_Btn_ZOOMIN, &CLegendDlg::OnBnClickedBtnZoomin)
	ON_BN_CLICKED(ID_Btn_ZOOMOUT, &CLegendDlg::OnBnClickedBtnZoomout)
	ON_BN_CLICKED(ID_Btn_SAVE, &CLegendDlg::OnBnClickedBtnSave)
/**********************************ColorButton扩展添加6********************************************/
	ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
	ON_MESSAGE(CPN_CLOSEUP,      OnCloseUp)
	ON_MESSAGE(CPN_DROPDOWN,     OnDropDown)
/**************************************************************************************************/
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Trans_StatisticArea, &CLegendDlg::OnCustomdrawSliderTransStatisticarea)
END_MESSAGE_MAP()


// CLegendDlg 消息处理程序


BOOL CLegendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

/**********************************List Control控件的初始化****************************************/
	CString str_StatisticArea;
	str_StatisticArea = ((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo;

    CRect rect;   
	
    // 获取编程语言列表视图控件的位置和大小   
    m_List_StatisticArea.GetClientRect(&rect);   
  
    // 为列表视图控件添加全行选中和栅格风格   
    m_List_StatisticArea.SetExtendedStyle(m_List_StatisticArea.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   
  
    // 为列表视图控件添加四列
	CString str_listTAB[] ={_T("中心经度"), _T("中心纬度"), _T("宽度"), _T("高度")};
	for(int i=0; i<sizeof(str_listTAB)/sizeof(str_listTAB[0]); i++)
	{
		m_List_StatisticArea.InsertColumn(i, str_listTAB[i], LVCFMT_CENTER, rect.Width()/4-5, i);//第一列的LVCFMT_CENTER参数无效 //-5是因为右面还有个垂直滚动条
	}

	//得到统计区域的个数（m_StasticArea_strInfo的格式为"4|116.5 40 0.5 0.5|114.5 37.5 0.5 0.5|117 37 0.5 0.5|118.5 38.5 0.5 0.5"）
	//对于这种不固定个数的情况，其实应该使用CStringArray的add(),可以参考（封装成函数）：http://blog.csdn.net/darkread/article/details/2456443
	//CString _SubItem(CString str, LPCTSTR lpszSe, int stI, int endI)，参考：http://bbs.csdn.net/topics/90372535
	//或者矢量的pushback()，可以参考（仅参考思路）：http://zhubangbing.blog.163.com/blog/static/52609270201011493645630/
	int nItem = 0 ;
	CString str_tmp;
	str_tmp=str_StatisticArea.Left(str_StatisticArea.Find('|'));
	swscanf_s(str_tmp.GetBuffer(0),_T("%d"),&nItem);


	float Lon,Lat,Width,Height;
	for(int i=0; i<nItem; i++)
	{
		//参考：http://yq1518602434.blog.163.com/blog/static/202937084201302741030311/
		//用于分割的只能是字符，不能是字符串。多个字符使用_tcstok函数 参考：http://crackren.iteye.com/blog/1148191
		AfxExtractSubString(str_tmp,(LPCTSTR)str_StatisticArea,i+1,'|');//i+1是由于4第一个是指的个数

		swscanf_s(str_tmp.GetBuffer(0),_T("%f%f%f%f"),&Lon,&Lat,&Width,&Height); 
		str_tmp.Format(_T("%.2f"),Lon);
		m_List_StatisticArea.InsertItem(i, str_tmp);  
		str_tmp.Format(_T("%.2f"),Lat);
		m_List_StatisticArea.SetItemText(i, 1, str_tmp);   
		str_tmp.Format(_T("%.2f"),Width);
		m_List_StatisticArea.SetItemText(i, 2, str_tmp); 
		str_tmp.Format(_T("%.2f"),Height);
		m_List_StatisticArea.SetItemText(i, 3, str_tmp); 
	}


	//解决ListCtrl控件第一列文字不能居中显示的问题 
	//我使用了第二种方法（插入第一列后，改变它的参数）
	//参考：http://www.cnblogs.com/zhuyf87/archive/2013/02/22/2922384.html
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT;
	m_List_StatisticArea.GetColumn(0, &lvc);
	lvc.fmt &=~ LVCFMT_JUSTIFYMASK; 
	lvc.fmt |= LVCFMT_CENTER;
	m_List_StatisticArea.SetColumn(0, &lvc);

	
/**************************************************************************************************/


/**********************************ColorButton扩展添加7********************************************/
	COLORREF shp_color_tmp;
	m_Shape_strInfo=((CLDFProcessDlg*)GetParent())->m_Shape_strInfo;

	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,0,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_epicenter); 
	m_ClrBtn_epicenter.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_epicenter.TrackSelection= TRUE;
	m_ClrBtn_epicenter.CustomText= _T("更多颜色...");
	m_ClrBtn_epicenter.DefaultText = _T("自动");


	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,1,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_provinborder); 
	m_ClrBtn_provinborder.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_provinborder.TrackSelection= TRUE;
	m_ClrBtn_provinborder.CustomText= _T("更多颜色...");
	m_ClrBtn_provinborder.DefaultText = _T("自动");


	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,2,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_fault); 
	m_ClrBtn_fault.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_fault.TrackSelection= TRUE;
	m_ClrBtn_fault.CustomText= _T("更多颜色...");
	m_ClrBtn_fault.DefaultText = _T("自动");

	//以下是对统计区域的颜色设置m_StasticArea_Color做处理
	shp_color_tmp = ((CLDFProcessDlg*)GetParent())->m_StasticArea_Color;//再次使用shp_color_tmp，这次实际上是获得统计区域的颜色设置
	m_ClrBtn_StasticArea.Color = COLORREF(0x00ffffff & shp_color_tmp);//对颜色做掩膜，以得到颜色（去掉透明度）
	m_ClrBtn_StasticArea.TrackSelection= TRUE;
	m_ClrBtn_StasticArea.CustomText= _T("更多颜色...");
	m_ClrBtn_StasticArea.DefaultText = _T("自动");

	m_Slider_StatisticArea_ColorTrans.SetRange(0,100); 
	m_Slider_StatisticArea_ColorTrans.SetPos( ((DWORD) (shp_color_tmp) >> 24)/2.55);//参考GdiPlusColor.h的GetAlpha()
/**************************************************************************************************/
	m_StasticArea_SelectStyle=((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle;//统计区域的样式（圆形or矩形or断裂）


	m_Channel=((CLDFProcessDlg*)GetParent())->m_Channel;
	m_dmin=((CLDFProcessDlg*)GetParent())->m_dmin;
	m_dmax=((CLDFProcessDlg*)GetParent())->m_dmax;
	m_dkmin=((CLDFProcessDlg*)GetParent())->m_dkmin;
	m_dkmax=((CLDFProcessDlg*)GetParent())->m_dkmax;
	m_fontsize_legend=((CLDFProcessDlg*)GetParent())->m_fontsize_legend;

	for(int i=0;i<((CLDFProcessDlg*)GetParent())->m_strArray.GetSize();i++){
	m_listFile_legend.AddString(((CLDFProcessDlg*)GetParent())->m_strArray[i]);
	}

	m_CHECK_epicenter=((CLDFProcessDlg*)GetParent())->m_CHECK_epicenter;
	m_CHECK_provin_border=((CLDFProcessDlg*)GetParent())->m_CHECK_provin_border;
	m_CHECK_fault=((CLDFProcessDlg*)GetParent())->m_CHECK_fault;

  
    m_COMO_SelChannel.AddString(_T("通道一"));   
	m_COMO_SelChannel.AddString(_T("通道二"));   
	m_COMO_SelChannel.AddString(_T("通道三"));     
    m_COMO_SelChannel.AddString(_T("通道四"));    
    m_COMO_SelChannel.AddString(_T("通道五"));  
    // 在组合框控件的列表框中索引为1的位置插入列表项“新浪”   
    //m_COMO_SelChannel.InsertString(1, _T("新浪"));   
  
    // 默认选择
    m_COMO_SelChannel.SetCurSel(m_Channel-1);//因为通道是从1到5，索引是从0到4

	UpdateData(FALSE);

	GetDlgItem(IDC_PIC_SHOW)->ShowWindow(FALSE);//隐藏图片控件以使控件的外边框不可见

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLegendDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//画图像
		CClientDC dc(GetDlgItem(IDC_PIC_SHOW));
		CSize sz = m_pLPic.GetImageSize();
		CRect rc(0, 0, (int)(sz.cx * m_rate), (int)(sz.cy * m_rate));
		m_pLPic.Render(&dc, rc);

		//画图例，这个图例指的是调色板，其他（下面的）图例指图像示例
		CClientDC dcl(GetDlgItem(IDC_PIC_LEGEND));
		GetDlgItem(IDC_PIC_LEGEND)->GetClientRect(&rc);
		m_pLPic.Renderlegend(&dcl , rc);
	}

}

//与CLDFProcessDlg中的OpenShape相同
BOOL CLegendDlg::OpenShape(LPCTSTR lpszPathName)
{
	if (this->m_pProvider_legend)
	{
		delete m_pProvider_legend;
		m_pProvider_legend = NULL;
	}
	this->m_pProvider_legend = new ShapeProvider(lpszPathName);
	return this->m_pProvider_legend->Open();

	IProvider * pProv = m_pProvider_legend;
	for (ShapeProvider::ITER_SHP iter = pProv->getGeometries().begin();
		iter < pProv->getGeometries().end();
		++iter)
	{
		const shp::ShapeObject * pShp = *iter;
		TRACE("%d(%d)-", pShp->Number(), pShp->Length());

		switch (pShp->ShapeType)
		{
		case shp::ShapeType::Point:
		{
			const shp::ShpPoint &pt = *static_cast<const shp::ShpPoint *>(pShp);
			TRACE("Point: (%3.10f,%3.10f)\n", pt.X, pt.Y);
		}
			break;
		case shp::ShapeType::Multipoint:
		{
			const shp::ShpMultiPoint &mpt = *static_cast<const shp::ShpMultiPoint *>(pShp);
			TRACE("MultiPoint: (");
			for (int i = 0; i<mpt.NumPoints; ++i)
				TRACE("(%f,%f)", mpt.Points[i].X, mpt.Points[i].Y);
			TRACE(")\n");
		}
			break;
		case shp::ShapeType::PolyLine:
		{
			const shp::ShpPolyLine& pl = *static_cast<const shp::ShpPolyLine *>(pShp);
			TRACE("PolyLine:\n");
			for (int i = 0; i<pl.NumParts; ++i)
			{
				TRACE("  Part(");
				int count = 0;
				const shp::Point* pts = pl.getPart(i, &count);
				for (int j = 0; j<count; ++j)
					TRACE("(%f,%f)", pts[j].X, pts[j].Y);
				TRACE(")\n");
			}
		}
			break;
		case shp::ShapeType::Polygon:
		{
			const shp::ShpPolygon& polygon = *static_cast<const shp::ShpPolygon *>(pShp);
			TRACE("Polygon: ");
			for (int i = 0; i<polygon.NumParts; ++i)
			{
				int count = 0;
				const shp::Point* pts = polygon.getPart(i, &count);
				TRACE("(%d)", count);
			}
			TRACE("\n");
		}
			break;
		}
	}

	return TRUE;

}


void CLegendDlg::OnSelchangeListFileLegend()
{
	// TODO: 在此添加控件通知处理程序代码
	//点击到list control的空白处是不需要重绘的
	UpdateData();
	BOOL success = draw_picture_showlegend();
	//点击空白处还是卡，原因不明
	if (success)
		Invalidate();
}


void CLegendDlg::OnBnClickedApplyLegend()
{
	// TODO: 在此添加控件通知处理程序代码
	//修改此处时注意应同步修改确定
	UpdateData();

	((CLDFProcessDlg*)GetParent())->m_Channel=m_Channel;
	((CLDFProcessDlg*)GetParent())->m_dmin=m_dmin;
	((CLDFProcessDlg*)GetParent())->m_dmax=m_dmax;
	((CLDFProcessDlg*)GetParent())->m_dkmin=m_dkmin;
	((CLDFProcessDlg*)GetParent())->m_dkmax=m_dkmax;
	((CLDFProcessDlg*)GetParent())->m_fontsize_legend=m_fontsize_legend;

	//%06x x打印16进制, 6最小域宽, 0左侧补'0'
	m_Shape_strInfo.Format(_T("0x%06x %d|0x%06x %d|0x%06x %d"),
		m_ClrBtn_epicenter.GetColor(),m_size_epicenter,m_ClrBtn_provinborder.GetColor(),m_size_provinborder,m_ClrBtn_fault.GetColor(),m_size_fault);
	((CLDFProcessDlg*)GetParent())->m_Shape_strInfo=m_Shape_strInfo;

	CString str_StatisticArea=_T("");
	str_StatisticArea.Format(_T("%d|"),m_List_StatisticArea.GetItemCount());
	for(int i=0;i<m_List_StatisticArea.GetItemCount();i++)
	{
		str_StatisticArea = str_StatisticArea + m_List_StatisticArea.GetItemText(i,0);
		for(int j=1;j<m_List_StatisticArea.GetHeaderCtrl()->GetItemCount();j++)
		{
			str_StatisticArea = str_StatisticArea + _T(" ") + m_List_StatisticArea.GetItemText(i,j);//获取i行j列的元素
		}
		if(i+1<m_List_StatisticArea.GetItemCount())
			str_StatisticArea = str_StatisticArea + _T("|");
	}
	((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo=str_StatisticArea;

	((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle=m_StasticArea_SelectStyle;

	//C++位运算参考：http://www.cnblogs.com/lzmfywz/archive/2013/03/16/2963873.html
	//不明白这里m_Slider_StatisticArea_ColorTrans.GetPos()*2.55会为254，当m_Slider_StatisticArea_ColorTrans.GetPos()为100时
	((CLDFProcessDlg*)GetParent())->m_StasticArea_Color = 
		(0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24) ;//参考GdiPlusColor.h的MakeARGB()

	((CLDFProcessDlg*)GetParent())->m_CHECK_epicenter=m_CHECK_epicenter;
	((CLDFProcessDlg*)GetParent())->m_CHECK_provin_border=m_CHECK_provin_border;
	((CLDFProcessDlg*)GetParent())->m_CHECK_fault=m_CHECK_fault;

	draw_picture_showlegend();
	Invalidate();
}


void CLegendDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//修改此处时注意应同步修改应用
	UpdateData();

	((CLDFProcessDlg*)GetParent())->m_Channel=m_Channel;
	((CLDFProcessDlg*)GetParent())->m_dmin=m_dmin;
	((CLDFProcessDlg*)GetParent())->m_dmax=m_dmax;
	((CLDFProcessDlg*)GetParent())->m_dkmin=m_dkmin;
	((CLDFProcessDlg*)GetParent())->m_dkmax=m_dkmax;
	((CLDFProcessDlg*)GetParent())->m_fontsize_legend=m_fontsize_legend;

	//%06x x打印16进制, 6最小域宽, 0左侧补'0'
	m_Shape_strInfo.Format(_T("0x%06x %d|0x%06x %d|0x%06x %d"),
		m_ClrBtn_epicenter.GetColor(),m_size_epicenter,m_ClrBtn_provinborder.GetColor(),m_size_provinborder,m_ClrBtn_fault.GetColor(),m_size_fault);
	((CLDFProcessDlg*)GetParent())->m_Shape_strInfo=m_Shape_strInfo;

	CString str_StatisticArea=_T("");
	str_StatisticArea.Format(_T("%d|"),m_List_StatisticArea.GetItemCount());
	for(int i=0;i<m_List_StatisticArea.GetItemCount();i++)
	{
		str_StatisticArea = str_StatisticArea + m_List_StatisticArea.GetItemText(i,0);
		for(int j=1;j<m_List_StatisticArea.GetHeaderCtrl()->GetItemCount();j++)
		{
			str_StatisticArea = str_StatisticArea + _T(" ") + m_List_StatisticArea.GetItemText(i,j);//获取i行j列的元素
		}
		if(i+1<m_List_StatisticArea.GetItemCount())
			str_StatisticArea = str_StatisticArea + _T("|");
	}
	((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo=str_StatisticArea;

	((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle=m_StasticArea_SelectStyle;

	//不明白这里m_Slider_StatisticArea_ColorTrans.GetPos()*2.55会为254，当m_Slider_StatisticArea_ColorTrans.GetPos()为100时
	((CLDFProcessDlg*)GetParent())->m_StasticArea_Color = 
		(0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24) ;//参考GdiPlusColor.h的MakeARGB()

	((CLDFProcessDlg*)GetParent())->m_CHECK_epicenter=m_CHECK_epicenter;
	((CLDFProcessDlg*)GetParent())->m_CHECK_provin_border=m_CHECK_provin_border;
	((CLDFProcessDlg*)GetParent())->m_CHECK_fault=m_CHECK_fault;

	CDialogEx::OnOK();
}


BOOL CLegendDlg::draw_picture_showlegend()
{
	LDF ldf1;
	CString str;
	int n = m_listFile_legend.GetCurSel();
	//如果选择了图像，则n不为-1，还有可能点到空白处
	if (n <= m_listFile_legend.GetCount()-1 && n!=-1)
	{
		m_listFile_legend.GetText(n, str);
		if (!ldf1.read(str)){
			return FALSE;
		}

		std::vector<CString> aShpnames;//魔数
		aShpnames.resize(3);
		aShpnames[0] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\provinborder.shp");//省界
		aShpnames[1] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\fault.shp");//断层
		aShpnames[2] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\epicenter.shp");//震中


		/*****             绘制矢量图层                 ******/
		m_pLPic.m_fontsize=m_fontsize_legend;//设置经纬度字体大小
		m_pLPic.setRange(m_Channel,m_dmin,m_dmax,m_fontsize_legend);
		m_pLPic.setLDF(ldf1);

		
		//解析m_Shape_strInfo//由CLegendDlg::OnInitDialog()修改
		COLORREF shp_color_epicenter,shp_color_provinborder,shp_color_fault;
		CString shp_strInfo=m_Shape_strInfo , str_tmp;
		int shp_size_epicenter, shp_size_provinborder, shp_size_fault;

		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,0,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_epicenter,&shp_size_epicenter); 

		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,1,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_provinborder,&shp_size_provinborder); 
	
		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,2,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_fault,&shp_size_fault); 
		//解析m_Shape_strInfo完毕

		//画shp的顺序是aShpnames[0]、[1]、[2]，而不是语句的顺序（因为本来就不需要用循环）
		//j==0 && 断层的关系已对应好，需要调整绘制shp时，aShpnames[i]与j==0两部分要同时调整
		for (size_t j = 0; j < aShpnames.size(); j++){
			OpenShape(aShpnames[j]);
			if (m_pProvider_legend){
				if (j==0 && m_CHECK_provin_border){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_provinborder),GetGValue(shp_color_provinborder),GetBValue(shp_color_provinborder)),shp_size_provinborder,1);}//省界
				if (j==1 && m_CHECK_fault){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_fault),GetGValue(shp_color_fault),GetBValue(shp_color_fault)),shp_size_fault,1);}//断层
				if (j==2 && m_CHECK_epicenter){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_epicenter),GetGValue(shp_color_epicenter),GetBValue(shp_color_epicenter)),shp_size_epicenter,2);}//震中
				//最后画外边缘，以覆盖超出绘图区域（即白色边框部分区域）的矢量图，可以放到循环外以提高效率
				//(m_pLPic).drawedge(ldf1);
			}
		}

		/*****             绘制统计区域                 ******/
		COLORREF color_arg = (0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24);
		switch (m_StasticArea_SelectStyle)
		{
			case 0://绘制圆形区域//因为与圆形区域一样，所以没有break;
			case 1://绘制矩形区域
				{
					for(int i=0;i<m_List_StatisticArea.GetItemCount();i++)
					{
						float Lon,Lat,Width,Height;
						Lon = _tstof(m_List_StatisticArea.GetItemText(i,0));
						Lat = _tstof(m_List_StatisticArea.GetItemText(i,1));
						Width = _tstof(m_List_StatisticArea.GetItemText(i,2));
						Height = _tstof(m_List_StatisticArea.GetItemText(i,3));
						(m_pLPic).draw_StatisticArea(ldf1,NULL,m_StasticArea_SelectStyle,color_arg,Lon,Lat,Width,Height);
					}
				}
				break;
			case 2://绘制断裂区域
				{
					OpenShape(((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\fault_statistic.shp"));
					(m_pLPic).draw_StatisticArea(ldf1,m_pProvider_legend,m_StasticArea_SelectStyle,color_arg,NULL,NULL,NULL,NULL);
				}
				break;
		}


		/*****             绘制制图边缘                 ******/
		(m_pLPic).drawedge(ldf1);


		//以下为picture Control显示图像的代码，保存图像与此无关
		CRect rect;
		GetDlgItem(IDC_PIC_SHOW)->GetWindowRect(&rect);
		CSize sz = m_pLPic.GetImageSize();
		double ratew = (double)rect.Width() / sz.cx * m_rate_Zoom;//* m_rate_Zoom是我自己加的，达到同步修改m_rate及IDC_PIC_SHOW的目的
		double rateh = (double)rect.Height() / sz.cy * m_rate_Zoom;

		//取m_rate为ratew和rateh中的小者
		ratew < rateh ? m_rate = ratew : m_rate = rateh;
		//存储m_rate的原始值，放大缩小时确定是否会超限
		if ( m_rate_origin ==-1)//因为对话框初始化的时候，把它设为了-1，这种方法很不规范
			m_rate_origin = m_rate;

		//使picture Control的区域为自适应缩放后的图像
		ScreenToClient(&rect); 
		GetDlgItem(IDC_PIC_SHOW)->MoveWindow(rect.left,rect.top,(int)(sz.cx * m_rate),(int)(sz.cy * m_rate),true);
		GetDlgItem(IDC_PIC_SHOW)->ShowWindow(TRUE);//将初始化时隐藏的图片控件显示

		return TRUE;
	}
	else
		return FALSE;
}


void CLegendDlg::OnBnClickedCheckepicenter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}


void CLegendDlg::OnBnClickedCheckprovinborder()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}


void CLegendDlg::OnBnClickedCheckfault()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
}

//Combo Box选取待处理通道
void CLegendDlg::OnSelchangeComboSelchannel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Channel = m_COMO_SelChannel.GetCurSel()+1;//因为通道是从1到5，索引是从0到4
}


void CLegendDlg::OnBnClickedBtnZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	if((m_rate /0.8)<=m_rate_origin)//初始显示时相当于最大化，只能比此小
	{
		m_rate_Zoom =1/0.8;
		draw_picture_showlegend();
		Invalidate();
	}

	m_rate_Zoom =1;//要把这个缩放变量设回1，否则一旦其他操作引起再次重绘，图像会再次缩放
}


void CLegendDlg::OnBnClickedBtnZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();

	m_rate_Zoom =0.8;
	draw_picture_showlegend();
	Invalidate();

	m_rate_Zoom =1;//要把这个缩放变量设回1，否则一旦其他操作引起再次重绘，图像会再次缩放
}


void CLegendDlg::OnBnClickedBtnSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter;
	strFilter = _T("PNG图像文件|*.png");
	CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
	if (IDOK != dlg.DoModal())
		return;

	CString strFileName;
	strFileName = dlg.m_ofn.lpstrFile;
	if (strFileName.Right(4) != _T(".png")){
		strFileName += _T(".png");
	}

	m_pLPic.Save(strFileName);

}

/**********************************ColorButton扩展添加8********************************************/
LONG CLegendDlg::OnSelEndOK(UINT /*lParam*/, LONG /*wParam*/)
{
    TRACE0("Selection ended OK\n");
    /*COLORREF m_Color = m_ClrBtn_epicenter.GetColor();  
    double red      = (double)GetRValue(m_Color);
    double green = (double)GetGValue(m_Color);
    double blue    = (double)GetBValue(m_Color);*/
    return TRUE;
}

LONG CLegendDlg::OnSelEndCancel(UINT /*lParam*/, LONG /*wParam*/)
{
    TRACE0("Selection cancelled\n");
    return TRUE;
}

LONG CLegendDlg::OnSelChange(UINT /*lParam*/, LONG /*wParam*/)
{
    TRACE0("Selection changed\n");
    return TRUE;
}

LONG CLegendDlg::OnCloseUp(UINT /*lParam*/, LONG /*wParam*/)
{
    TRACE0("Colour picker close up\n");
    return TRUE;
}

LONG CLegendDlg::OnDropDown(UINT /*lParam*/, LONG /*wParam*/)
{
    TRACE0("Colour picker drop down\n");
    return TRUE;
}
/**************************************************************************************************/



//用以截获Delete与Enter按键消息，并处理
//参考：http://zhidao.baidu.com/link?url=ShJU2Sd1Qyqz1mwR8OIoZnRmkK8_STbFRiMlRmdvcu9Fft2RP1RFuoBV9KPSsx9XEFa9v-2e2T2N2Uh7-MJTUa
//参考：http://bbs.csdn.net/topics/43276
BOOL CLegendDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	//处理Enter
	if (WM_KEYDOWN   ==   pMsg-> message && VK_RETURN  ==   pMsg-> wParam)
	{
		if (  GetDlgItem(IDC_EDIT_AddStatisticArea) == GetFocus())
		{
			CString str_tmp;
			float Lon,Lat,Width,Height;
			GetDlgItem(IDC_EDIT_AddStatisticArea)->GetWindowText(str_tmp);

			//最初想法：sscanf(s,"%[^,],%[^,],%[^,],%[^,]",b,c,d); 参考：http://bbs.csdn.net/topics/90372535
			//sscanf，sscanf_s及其相关用法：http://www.cnblogs.com/kex1n/archive/2011/06/09/2076501.html
			//其实应该对输入加以判断，有个类似资料：http://zhidao.baidu.com/link?url=K3d8GodvCgTbtONa4pYQMWG2PVzrwjiGrFbAeTwpdwbiRh8DTxIlYYKvWSxWeEm2lHGqSpIQYsRfCcysUArxI_
			swscanf_s(str_tmp.GetBuffer(0),_T("%f%f%f%f"),&Lon,&Lat,&Width,&Height); ; 
			//str.Format(_T("%f,%f,%f,%f"),Lon,Lat,Width,Height);
			//MessageBox(str);
			str_tmp.Format(_T("%.2f"),Lon);
			m_List_StatisticArea.InsertItem(0, str_tmp);  
			str_tmp.Format(_T("%.2f"),Lat);
			m_List_StatisticArea.SetItemText(0, 1, str_tmp);   
			str_tmp.Format(_T("%.2f"),Width);
			m_List_StatisticArea.SetItemText(0, 2, str_tmp); 
			str_tmp.Format(_T("%.2f"),Height);
			m_List_StatisticArea.SetItemText(0, 3, str_tmp); 


			GetDlgItem(IDC_EDIT_AddStatisticArea)->SetWindowText(_T(""));

			return FALSE;//还不清楚与return TRUE的区别;
		}

	}

	//处理Delete，最好改成从后往前删，参考：http://bbs.csdn.net/topics/190114396
	if (WM_KEYDOWN   ==   pMsg-> message && VK_DELETE  ==   pMsg-> wParam)
	{
		if (  GetDlgItem(IDC_LIST_StatisticArea) == GetFocus())
		{
			POSITION pos =m_List_StatisticArea.GetFirstSelectedItemPosition();//获取首选中行位置
			while (pos) 
			{
				int  nSelected=m_List_StatisticArea.GetNextSelectedItem(pos); //获取选中行的索引
				m_List_StatisticArea.DeleteItem(nSelected); //根据索引删除
				pos  = m_List_StatisticArea.GetFirstSelectedItemPosition();  //这个语句重新计算选中行在删掉一条后的listctl中位置
			}

		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//Slider Control的控制，参考：http://bbs.csdn.net/topics/360240616
//详细资料，参考：http://blog.csdn.net/wangjiannuaa/article/details/6306176
//详细资料，参考：http://javababy1.iteye.com/blog/1297980
void CLegendDlg::OnCustomdrawSliderTransStatisticarea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

    int nPos = m_Slider_StatisticArea_ColorTrans.GetPos();
    CString strText;
    strText.Format(_T("透明度 :%d"),nPos);
    GetDlgItem(IDC_STATIC_StatisticArea_ColorTrans)->SetWindowText(strText);

	*pResult = 0;
}
