// LegendDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "LegendDlg.h"
#include "afxdialogex.h"
#include "LDFProcessDlg.h"
#include ".\lib\ShapeProvider.h"


// CLegendDlg �Ի���

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
	, m_rate_origin(-1.0)//ע�����ǧ��Ҫ�޸ģ���������ж��ǲ��ǵ�һ�γ�ʼ��
	, m_Shape_strInfo(_T(""))
	//, m_StasticArea_strInfo(0)//ע�⣬m_List_StatisticArea��LDFProcess �� m_StasticArea_strInfo���������LegendDlg�в����ڸñ���
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


/*	DDX_TEXT()		�����ÿ������Ϊ���ַ��������Ϳؼ����ı���WindowText������������
	DDX_Control()	�����ÿ������Ϊ�ѱ����Ϳؼ��������������
	DoDataExchange(pDX)	���Ǵ������б�����������ؼ��������ݵĺ�����
	�ڳ�����
	DDX_Text(pDX, IDC_ID_VALUE, m_strID);
	DDX_Control(pDX, IDC_ID_VALUE, m_editID);//ͨ���޸ı�������ֵ���ƿؼ�����(�����໥����)
	ͨ��update(TRUE)ȡ�ÿؼ��ϵ�ֵ��m_strID�������޸ĺ�ͨ��update(FALSE)���ؿؼ���������ʾ
	ͨ��m_editID.GetWindText(str),ȡ��ֵ�������ͨ��m_editID.SetWindText(str)���ؿؼ�������ʾ
	��IDΪIDC_ID_VALUEO�Ŀؼ������������m_strID���а󶨡�
	����UpdateData��TRUE��ʱ�����ؼ������ָ�ֵ������
	����UpdateData��FALSE��ʱ����������ֵ��ֵ���ؼ�������
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
	/**********************************ColorButton��չ���5********************************************/
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
/**********************************ColorButton��չ���6********************************************/
	ON_MESSAGE(CPN_SELENDOK,     OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELCHANGE,    OnSelChange)
	ON_MESSAGE(CPN_CLOSEUP,      OnCloseUp)
	ON_MESSAGE(CPN_DROPDOWN,     OnDropDown)
/**************************************************************************************************/
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Trans_StatisticArea, &CLegendDlg::OnCustomdrawSliderTransStatisticarea)
END_MESSAGE_MAP()


// CLegendDlg ��Ϣ�������


BOOL CLegendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

/**********************************List Control�ؼ��ĳ�ʼ��****************************************/
	CString str_StatisticArea;
	str_StatisticArea = ((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo;

    CRect rect;   
	
    // ��ȡ��������б���ͼ�ؼ���λ�úʹ�С   
    m_List_StatisticArea.GetClientRect(&rect);   
  
    // Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����   
    m_List_StatisticArea.SetExtendedStyle(m_List_StatisticArea.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);   
  
    // Ϊ�б���ͼ�ؼ��������
	CString str_listTAB[] ={_T("���ľ���"), _T("����γ��"), _T("���"), _T("�߶�")};
	for(int i=0; i<sizeof(str_listTAB)/sizeof(str_listTAB[0]); i++)
	{
		m_List_StatisticArea.InsertColumn(i, str_listTAB[i], LVCFMT_CENTER, rect.Width()/4-5, i);//��һ�е�LVCFMT_CENTER������Ч //-5����Ϊ���滹�и���ֱ������
	}

	//�õ�ͳ������ĸ�����m_StasticArea_strInfo�ĸ�ʽΪ"4|116.5 40 0.5 0.5|114.5 37.5 0.5 0.5|117 37 0.5 0.5|118.5 38.5 0.5 0.5"��
	//�������ֲ��̶��������������ʵӦ��ʹ��CStringArray��add(),���Բο�����װ�ɺ�������http://blog.csdn.net/darkread/article/details/2456443
	//CString _SubItem(CString str, LPCTSTR lpszSe, int stI, int endI)���ο���http://bbs.csdn.net/topics/90372535
	//����ʸ����pushback()�����Բο������ο�˼·����http://zhubangbing.blog.163.com/blog/static/52609270201011493645630/
	int nItem = 0 ;
	CString str_tmp;
	str_tmp=str_StatisticArea.Left(str_StatisticArea.Find('|'));
	swscanf_s(str_tmp.GetBuffer(0),_T("%d"),&nItem);


	float Lon,Lat,Width,Height;
	for(int i=0; i<nItem; i++)
	{
		//�ο���http://yq1518602434.blog.163.com/blog/static/202937084201302741030311/
		//���ڷָ��ֻ�����ַ����������ַ���������ַ�ʹ��_tcstok���� �ο���http://crackren.iteye.com/blog/1148191
		AfxExtractSubString(str_tmp,(LPCTSTR)str_StatisticArea,i+1,'|');//i+1������4��һ����ָ�ĸ���

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


	//���ListCtrl�ؼ���һ�����ֲ��ܾ�����ʾ������ 
	//��ʹ���˵ڶ��ַ����������һ�к󣬸ı����Ĳ�����
	//�ο���http://www.cnblogs.com/zhuyf87/archive/2013/02/22/2922384.html
	LVCOLUMN lvc;
	lvc.mask = LVCF_FMT;
	m_List_StatisticArea.GetColumn(0, &lvc);
	lvc.fmt &=~ LVCFMT_JUSTIFYMASK; 
	lvc.fmt |= LVCFMT_CENTER;
	m_List_StatisticArea.SetColumn(0, &lvc);

	
/**************************************************************************************************/


/**********************************ColorButton��չ���7********************************************/
	COLORREF shp_color_tmp;
	m_Shape_strInfo=((CLDFProcessDlg*)GetParent())->m_Shape_strInfo;

	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,0,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_epicenter); 
	m_ClrBtn_epicenter.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_epicenter.TrackSelection= TRUE;
	m_ClrBtn_epicenter.CustomText= _T("������ɫ...");
	m_ClrBtn_epicenter.DefaultText = _T("�Զ�");


	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,1,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_provinborder); 
	m_ClrBtn_provinborder.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_provinborder.TrackSelection= TRUE;
	m_ClrBtn_provinborder.CustomText= _T("������ɫ...");
	m_ClrBtn_provinborder.DefaultText = _T("�Զ�");


	AfxExtractSubString(str_tmp,(LPCTSTR)m_Shape_strInfo,2,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_tmp,&m_size_fault); 
	m_ClrBtn_fault.Color = COLORREF(shp_color_tmp);
	m_ClrBtn_fault.TrackSelection= TRUE;
	m_ClrBtn_fault.CustomText= _T("������ɫ...");
	m_ClrBtn_fault.DefaultText = _T("�Զ�");

	//�����Ƕ�ͳ���������ɫ����m_StasticArea_Color������
	shp_color_tmp = ((CLDFProcessDlg*)GetParent())->m_StasticArea_Color;//�ٴ�ʹ��shp_color_tmp�����ʵ�����ǻ��ͳ���������ɫ����
	m_ClrBtn_StasticArea.Color = COLORREF(0x00ffffff & shp_color_tmp);//����ɫ����Ĥ���Եõ���ɫ��ȥ��͸���ȣ�
	m_ClrBtn_StasticArea.TrackSelection= TRUE;
	m_ClrBtn_StasticArea.CustomText= _T("������ɫ...");
	m_ClrBtn_StasticArea.DefaultText = _T("�Զ�");

	m_Slider_StatisticArea_ColorTrans.SetRange(0,100); 
	m_Slider_StatisticArea_ColorTrans.SetPos( ((DWORD) (shp_color_tmp) >> 24)/2.55);//�ο�GdiPlusColor.h��GetAlpha()
/**************************************************************************************************/
	m_StasticArea_SelectStyle=((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle;//ͳ���������ʽ��Բ��or����or���ѣ�


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

  
    m_COMO_SelChannel.AddString(_T("ͨ��һ"));   
	m_COMO_SelChannel.AddString(_T("ͨ����"));   
	m_COMO_SelChannel.AddString(_T("ͨ����"));     
    m_COMO_SelChannel.AddString(_T("ͨ����"));    
    m_COMO_SelChannel.AddString(_T("ͨ����"));  
    // ����Ͽ�ؼ����б��������Ϊ1��λ�ò����б�����ˡ�   
    //m_COMO_SelChannel.InsertString(1, _T("����"));   
  
    // Ĭ��ѡ��
    m_COMO_SelChannel.SetCurSel(m_Channel-1);//��Ϊͨ���Ǵ�1��5�������Ǵ�0��4

	UpdateData(FALSE);

	GetDlgItem(IDC_PIC_SHOW)->ShowWindow(FALSE);//����ͼƬ�ؼ���ʹ�ؼ�����߿򲻿ɼ�

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CLegendDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		//dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//��ͼ��
		CClientDC dc(GetDlgItem(IDC_PIC_SHOW));
		CSize sz = m_pLPic.GetImageSize();
		CRect rc(0, 0, (int)(sz.cx * m_rate), (int)(sz.cy * m_rate));
		m_pLPic.Render(&dc, rc);

		//��ͼ�������ͼ��ָ���ǵ�ɫ�壬����������ģ�ͼ��ָͼ��ʾ��
		CClientDC dcl(GetDlgItem(IDC_PIC_LEGEND));
		GetDlgItem(IDC_PIC_LEGEND)->GetClientRect(&rc);
		m_pLPic.Renderlegend(&dcl , rc);
	}

}

//��CLDFProcessDlg�е�OpenShape��ͬ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�����list control�Ŀհ״��ǲ���Ҫ�ػ��
	UpdateData();
	BOOL success = draw_picture_showlegend();
	//����հ״����ǿ���ԭ����
	if (success)
		Invalidate();
}


void CLegendDlg::OnBnClickedApplyLegend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�޸Ĵ˴�ʱע��Ӧͬ���޸�ȷ��
	UpdateData();

	((CLDFProcessDlg*)GetParent())->m_Channel=m_Channel;
	((CLDFProcessDlg*)GetParent())->m_dmin=m_dmin;
	((CLDFProcessDlg*)GetParent())->m_dmax=m_dmax;
	((CLDFProcessDlg*)GetParent())->m_dkmin=m_dkmin;
	((CLDFProcessDlg*)GetParent())->m_dkmax=m_dkmax;
	((CLDFProcessDlg*)GetParent())->m_fontsize_legend=m_fontsize_legend;

	//%06x x��ӡ16����, 6��С���, 0��ಹ'0'
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
			str_StatisticArea = str_StatisticArea + _T(" ") + m_List_StatisticArea.GetItemText(i,j);//��ȡi��j�е�Ԫ��
		}
		if(i+1<m_List_StatisticArea.GetItemCount())
			str_StatisticArea = str_StatisticArea + _T("|");
	}
	((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo=str_StatisticArea;

	((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle=m_StasticArea_SelectStyle;

	//C++λ����ο���http://www.cnblogs.com/lzmfywz/archive/2013/03/16/2963873.html
	//����������m_Slider_StatisticArea_ColorTrans.GetPos()*2.55��Ϊ254����m_Slider_StatisticArea_ColorTrans.GetPos()Ϊ100ʱ
	((CLDFProcessDlg*)GetParent())->m_StasticArea_Color = 
		(0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24) ;//�ο�GdiPlusColor.h��MakeARGB()

	((CLDFProcessDlg*)GetParent())->m_CHECK_epicenter=m_CHECK_epicenter;
	((CLDFProcessDlg*)GetParent())->m_CHECK_provin_border=m_CHECK_provin_border;
	((CLDFProcessDlg*)GetParent())->m_CHECK_fault=m_CHECK_fault;

	draw_picture_showlegend();
	Invalidate();
}


void CLegendDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�޸Ĵ˴�ʱע��Ӧͬ���޸�Ӧ��
	UpdateData();

	((CLDFProcessDlg*)GetParent())->m_Channel=m_Channel;
	((CLDFProcessDlg*)GetParent())->m_dmin=m_dmin;
	((CLDFProcessDlg*)GetParent())->m_dmax=m_dmax;
	((CLDFProcessDlg*)GetParent())->m_dkmin=m_dkmin;
	((CLDFProcessDlg*)GetParent())->m_dkmax=m_dkmax;
	((CLDFProcessDlg*)GetParent())->m_fontsize_legend=m_fontsize_legend;

	//%06x x��ӡ16����, 6��С���, 0��ಹ'0'
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
			str_StatisticArea = str_StatisticArea + _T(" ") + m_List_StatisticArea.GetItemText(i,j);//��ȡi��j�е�Ԫ��
		}
		if(i+1<m_List_StatisticArea.GetItemCount())
			str_StatisticArea = str_StatisticArea + _T("|");
	}
	((CLDFProcessDlg*)GetParent())->m_StasticArea_strInfo=str_StatisticArea;

	((CLDFProcessDlg*)GetParent())->m_StasticArea_SelectStyle=m_StasticArea_SelectStyle;

	//����������m_Slider_StatisticArea_ColorTrans.GetPos()*2.55��Ϊ254����m_Slider_StatisticArea_ColorTrans.GetPos()Ϊ100ʱ
	((CLDFProcessDlg*)GetParent())->m_StasticArea_Color = 
		(0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24) ;//�ο�GdiPlusColor.h��MakeARGB()

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
	//���ѡ����ͼ����n��Ϊ-1�����п��ܵ㵽�հ״�
	if (n <= m_listFile_legend.GetCount()-1 && n!=-1)
	{
		m_listFile_legend.GetText(n, str);
		if (!ldf1.read(str)){
			return FALSE;
		}

		std::vector<CString> aShpnames;//ħ��
		aShpnames.resize(3);
		aShpnames[0] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\provinborder.shp");//ʡ��
		aShpnames[1] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\fault.shp");//�ϲ�
		aShpnames[2] = ((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\epicenter.shp");//����


		/*****             ����ʸ��ͼ��                 ******/
		m_pLPic.m_fontsize=m_fontsize_legend;//���þ�γ�������С
		m_pLPic.setRange(m_Channel,m_dmin,m_dmax,m_fontsize_legend);
		m_pLPic.setLDF(ldf1);

		
		//����m_Shape_strInfo//��CLegendDlg::OnInitDialog()�޸�
		COLORREF shp_color_epicenter,shp_color_provinborder,shp_color_fault;
		CString shp_strInfo=m_Shape_strInfo , str_tmp;
		int shp_size_epicenter, shp_size_provinborder, shp_size_fault;

		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,0,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_epicenter,&shp_size_epicenter); 

		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,1,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_provinborder,&shp_size_provinborder); 
	
		AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,2,'|');
		swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_fault,&shp_size_fault); 
		//����m_Shape_strInfo���

		//��shp��˳����aShpnames[0]��[1]��[2]������������˳����Ϊ�����Ͳ���Ҫ��ѭ����
		//j==0 && �ϲ�Ĺ�ϵ�Ѷ�Ӧ�ã���Ҫ��������shpʱ��aShpnames[i]��j==0������Ҫͬʱ����
		for (size_t j = 0; j < aShpnames.size(); j++){
			OpenShape(aShpnames[j]);
			if (m_pProvider_legend){
				if (j==0 && m_CHECK_provin_border){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_provinborder),GetGValue(shp_color_provinborder),GetBValue(shp_color_provinborder)),shp_size_provinborder,1);}//ʡ��
				if (j==1 && m_CHECK_fault){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_fault),GetGValue(shp_color_fault),GetBValue(shp_color_fault)),shp_size_fault,1);}//�ϲ�
				if (j==2 && m_CHECK_epicenter){(m_pLPic).DrawMap_arg(m_pProvider_legend,
					Gdiplus::Color(GetRValue(shp_color_epicenter),GetGValue(shp_color_epicenter),GetBValue(shp_color_epicenter)),shp_size_epicenter,2);}//����
				//������Ե���Ը��ǳ�����ͼ���򣨼���ɫ�߿򲿷����򣩵�ʸ��ͼ�����Էŵ�ѭ���������Ч��
				//(m_pLPic).drawedge(ldf1);
			}
		}

		/*****             ����ͳ������                 ******/
		COLORREF color_arg = (0x00ffffff & (DWORD) ( m_ClrBtn_StasticArea.GetColor() )) |  (0xff000000 & (DWORD) (m_Slider_StatisticArea_ColorTrans.GetPos()*2.55)<<24);
		switch (m_StasticArea_SelectStyle)
		{
			case 0://����Բ������//��Ϊ��Բ������һ��������û��break;
			case 1://���ƾ�������
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
			case 2://���ƶ�������
				{
					OpenShape(((CLDFProcessDlg*)GetParent())->GetAppPath() + _T("Config\\fault_statistic.shp"));
					(m_pLPic).draw_StatisticArea(ldf1,m_pProvider_legend,m_StasticArea_SelectStyle,color_arg,NULL,NULL,NULL,NULL);
				}
				break;
		}


		/*****             ������ͼ��Ե                 ******/
		(m_pLPic).drawedge(ldf1);


		//����Ϊpicture Control��ʾͼ��Ĵ��룬����ͼ������޹�
		CRect rect;
		GetDlgItem(IDC_PIC_SHOW)->GetWindowRect(&rect);
		CSize sz = m_pLPic.GetImageSize();
		double ratew = (double)rect.Width() / sz.cx * m_rate_Zoom;//* m_rate_Zoom�����Լ��ӵģ��ﵽͬ���޸�m_rate��IDC_PIC_SHOW��Ŀ��
		double rateh = (double)rect.Height() / sz.cy * m_rate_Zoom;

		//ȡm_rateΪratew��rateh�е�С��
		ratew < rateh ? m_rate = ratew : m_rate = rateh;
		//�洢m_rate��ԭʼֵ���Ŵ���Сʱȷ���Ƿ�ᳬ��
		if ( m_rate_origin ==-1)//��Ϊ�Ի����ʼ����ʱ�򣬰�����Ϊ��-1�����ַ����ܲ��淶
			m_rate_origin = m_rate;

		//ʹpicture Control������Ϊ����Ӧ���ź��ͼ��
		ScreenToClient(&rect); 
		GetDlgItem(IDC_PIC_SHOW)->MoveWindow(rect.left,rect.top,(int)(sz.cx * m_rate),(int)(sz.cy * m_rate),true);
		GetDlgItem(IDC_PIC_SHOW)->ShowWindow(TRUE);//����ʼ��ʱ���ص�ͼƬ�ؼ���ʾ

		return TRUE;
	}
	else
		return FALSE;
}


void CLegendDlg::OnBnClickedCheckepicenter()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CLegendDlg::OnBnClickedCheckprovinborder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}


void CLegendDlg::OnBnClickedCheckfault()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
}

//Combo Boxѡȡ������ͨ��
void CLegendDlg::OnSelchangeComboSelchannel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Channel = m_COMO_SelChannel.GetCurSel()+1;//��Ϊͨ���Ǵ�1��5�������Ǵ�0��4
}


void CLegendDlg::OnBnClickedBtnZoomin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	if((m_rate /0.8)<=m_rate_origin)//��ʼ��ʾʱ�൱����󻯣�ֻ�ܱȴ�С
	{
		m_rate_Zoom =1/0.8;
		draw_picture_showlegend();
		Invalidate();
	}

	m_rate_Zoom =1;//Ҫ��������ű������1������һ���������������ٴ��ػ棬ͼ����ٴ�����
}


void CLegendDlg::OnBnClickedBtnZoomout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();

	m_rate_Zoom =0.8;
	draw_picture_showlegend();
	Invalidate();

	m_rate_Zoom =1;//Ҫ��������ű������1������һ���������������ٴ��ػ棬ͼ����ٴ�����
}


void CLegendDlg::OnBnClickedBtnSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strFilter;
	strFilter = _T("PNGͼ���ļ�|*.png");
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

/**********************************ColorButton��չ���8********************************************/
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



//���Խػ�Delete��Enter������Ϣ��������
//�ο���http://zhidao.baidu.com/link?url=ShJU2Sd1Qyqz1mwR8OIoZnRmkK8_STbFRiMlRmdvcu9Fft2RP1RFuoBV9KPSsx9XEFa9v-2e2T2N2Uh7-MJTUa
//�ο���http://bbs.csdn.net/topics/43276
BOOL CLegendDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//����Enter
	if (WM_KEYDOWN   ==   pMsg-> message && VK_RETURN  ==   pMsg-> wParam)
	{
		if (  GetDlgItem(IDC_EDIT_AddStatisticArea) == GetFocus())
		{
			CString str_tmp;
			float Lon,Lat,Width,Height;
			GetDlgItem(IDC_EDIT_AddStatisticArea)->GetWindowText(str_tmp);

			//����뷨��sscanf(s,"%[^,],%[^,],%[^,],%[^,]",b,c,d); �ο���http://bbs.csdn.net/topics/90372535
			//sscanf��sscanf_s��������÷���http://www.cnblogs.com/kex1n/archive/2011/06/09/2076501.html
			//��ʵӦ�ö���������жϣ��и��������ϣ�http://zhidao.baidu.com/link?url=K3d8GodvCgTbtONa4pYQMWG2PVzrwjiGrFbAeTwpdwbiRh8DTxIlYYKvWSxWeEm2lHGqSpIQYsRfCcysUArxI_
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

			return FALSE;//���������return TRUE������;
		}

	}

	//����Delete����øĳɴӺ���ǰɾ���ο���http://bbs.csdn.net/topics/190114396
	if (WM_KEYDOWN   ==   pMsg-> message && VK_DELETE  ==   pMsg-> wParam)
	{
		if (  GetDlgItem(IDC_LIST_StatisticArea) == GetFocus())
		{
			POSITION pos =m_List_StatisticArea.GetFirstSelectedItemPosition();//��ȡ��ѡ����λ��
			while (pos) 
			{
				int  nSelected=m_List_StatisticArea.GetNextSelectedItem(pos); //��ȡѡ���е�����
				m_List_StatisticArea.DeleteItem(nSelected); //��������ɾ��
				pos  = m_List_StatisticArea.GetFirstSelectedItemPosition();  //���������¼���ѡ������ɾ��һ�����listctl��λ��
			}

		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//Slider Control�Ŀ��ƣ��ο���http://bbs.csdn.net/topics/360240616
//��ϸ���ϣ��ο���http://blog.csdn.net/wangjiannuaa/article/details/6306176
//��ϸ���ϣ��ο���http://javababy1.iteye.com/blog/1297980
void CLegendDlg::OnCustomdrawSliderTransStatisticarea(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

    int nPos = m_Slider_StatisticArea_ColorTrans.GetPos();
    CString strText;
    strText.Format(_T("͸���� :%d"),nPos);
    GetDlgItem(IDC_STATIC_StatisticArea_ColorTrans)->SetWindowText(strText);

	*pResult = 0;
}
