// LDFProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "LDFProcessDlg.h"
#include "AbnmdlDlg.h"
#include "CldetcDlg.h"
#include "InputDlg.h"
#include "LegendDlg.h"
#include "OutputDlg.h"
#include ".\lib\LDF.h"
#include "gdal_priv.h"
#include ".\lib\ShapeProvider.h"
#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLDFProcessDlg dialog

CLDFProcessDlg::CLDFProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLDFProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLDFProcessDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	GetConSetting();//�ɳ�����config.ini�ĳ�ʼ������

	m_fTime=0.0;
	m_pProvider = NULL;

}

CLDFProcessDlg::~CLDFProcessDlg()
{
	if (m_pProvider)
	{
		delete m_pProvider;
		m_pProvider = NULL;
	}
}

void CLDFProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLDFProcessDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLDFProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CLDFProcessDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUT_Browser, OnBUTBrowser)
	ON_BN_CLICKED(IDC_BUT_Input, OnBUTInput)
	ON_BN_CLICKED(IDC_BUT_CLOUDetection, OnBUTCLOUDetection)
	ON_BN_CLICKED(IDC_BUT_Abnormal, OnBUTAbnormal)
	ON_BN_CLICKED(IDC_BUT_Output, OnBUTOutput)
	ON_BN_CLICKED(IDC_BUT_CAL, OnButCalculate)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUT_Legend, &CLDFProcessDlg::OnClickedButLegend)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLDFProcessDlg message handlers

BOOL CLDFProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLDFProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLDFProcessDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLDFProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CLDFProcessDlg::OpenShape(LPCTSTR lpszPathName)
{
	if (this->m_pProvider)
	{
		delete m_pProvider;
		m_pProvider = NULL;
	}
	this->m_pProvider = new ShapeProvider(lpszPathName);
	return this->m_pProvider->Open();

	/*�����return˵�����沿�ֶ�û���ˣ�������ֱ��ע�͵���
	IProvider * pProv = m_pProvider;
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

	return TRUE;*/

}
/*
void CLDFProcessDlg::GeoToMap(const shp::Point * geoPts, POINT * pts, int count)
{
	for (int i = 0; i<count; ++i)
	{
		pts[i].x = (int)((geoPts[i].X - m_origin.X) * m_ratioX);
		pts[i].y = (int)((m_origin.Y - geoPts[i].Y) * m_ratioY);
	}
}

void CLDFProcessDlg::GeoToMap(double geoX, double geoY, POINT * pt)
{
	pt->x = (int)((geoX - m_origin.X) * m_ratioX);
	pt->y = (int)((m_origin.Y - geoY) * m_ratioY);
}

void CLDFProcessDlg::MapToGeo(int x, int y, Point2D * geoPoint)
{
	geoPoint->X = (x / m_ratioX) + m_origin.X;
	geoPoint->Y = m_origin.Y - (y / m_ratioY);
}

void CLDFProcessDlg::InitMap(IProvider * pProvider)
{
	// view size
	CRect rect;
	this->GetClientRect(&rect);
	if (!m_pLPic.m_pImage){
	// map extents
		Rectangle2D ext = pProvider->getExtents();
		if (ext.IsEmpty())
		{
			ext.Xmax += 0.01;
			ext.Ymax += 0.01;
		}

		Rectangle2D bounds;
		bounds.Xmin = ext.Xmin - ext.getWidth() / 5;
		bounds.Xmax = ext.Xmax + ext.getWidth() / 5;
		bounds.Ymin = ext.Ymin - ext.getHeight() / 5;
		bounds.Ymax = ext.Ymax + ext.getHeight() / 5;

		m_origin = Point2D(bounds.Xmin, bounds.Ymax);
		m_ratioX = min(rect.Width() / bounds.getWidth(), rect.Height() / bounds.getHeight());
		m_ratioY = min(rect.Width() / bounds.getWidth(), rect.Height() / bounds.getHeight());
	}
	else{
	//ע��ԭ����32��ָ�߿��ȣ�����Ϊ����
		m_origin = Point2D(m_pLPic.m_dMinLon - 32 / m_pLPic.m_dratioX, m_pLPic.m_dMaxLat + 32 / m_pLPic.m_dratioY);
		m_ratioX = m_pLPic.m_dratioX * m_rate;
		m_ratioY = m_pLPic.m_dratioY * m_rate;
	}
	return;
	
}


void CLDFProcessDlg::DrawMap(CDC* pDC, IProvider * pProvider)
{
	this->InitMap(pProvider);

	// draw

	CBrush	NewBrush;
	CBrush *OldBrush;
	NewBrush.CreateSolidBrush(RGB(255, 0, 0));
	OldBrush = pDC->SelectObject(&NewBrush);

	std::vector<const shp::ShapeObject *> vecShp = pProvider->getGeometries();
	for (IProvider::ITER_SHP iter = vecShp.begin();
		iter < vecShp.end();
		iter++)
	{
		const shp::ShapeObject * pShp = *iter;
		switch (pShp->ShapeType)
		{
		case shp::ShapeType::Point:
		{
			const shp::ShpPoint &shpPt = *static_cast<const shp::ShpPoint *>(pShp);
			POINT pt;
			this->GeoToMap(shpPt.X, shpPt.Y, &pt);
			pDC->Ellipse(pt.x - 5, pt.y + 5, pt.x + 5, pt.y - 5);
			//pDC->FloodFill( pt.x, pt.y, RGB(255, 0, 0) );
		}
			break;
		case shp::ShapeType::Multipoint:
		{
			const shp::ShpMultiPoint &mpt = *static_cast<const shp::ShpMultiPoint *>(pShp);
			POINT pt;
			for (int i = 0; i<mpt.NumPoints; ++i)
			{
				this->GeoToMap(mpt.Points[i].X, mpt.Points[i].Y, &pt);
				pDC->Ellipse(pt.x - 5, pt.y + 5, pt.x + 5, pt.y - 5);
			}
		}
			break;
		case shp::ShapeType::PolyLine:
		{
			const shp::ShpPolyLine& pl = *static_cast<const shp::ShpPolyLine *>(pShp);
			for (int i = 0; i<pl.NumParts; ++i)
			{
				int count = 0;
				const shp::Point* pts = pl.getPart(i, &count);
				if (count > 0)
				{
					POINT pt;
					this->GeoToMap(pts[0].X, pts[0].Y, &pt);
					pDC->MoveTo(pt.x, pt.y);
					for (int j = 0; j<count; ++j)
					{
						this->GeoToMap(pts[j].X, pts[j].Y, &pt);
						pDC->LineTo(pt.x, pt.y);
					}
				}
			}
		}
			break;
		case shp::ShapeType::Polygon:
		{
			const shp::ShpPolygon& polygon = *static_cast<const shp::ShpPolygon *>(pShp);
			int NumParts = polygon.NumParts;
			int NumPoints = polygon.NumPoints;
			int * counts = new int[polygon.NumParts];
			POINT * pts = new POINT[polygon.NumPoints];

			this->GeoToMap(&polygon.Points(0), pts, polygon.NumPoints);
			for (int i = 0; i<polygon.NumParts - 1; ++i)
				counts[i] = polygon.Parts[i + 1] - polygon.Parts[i];
			counts[polygon.NumParts - 1] = polygon.NumPoints - polygon.Parts[polygon.NumParts - 1];

			//CRgn rgn;
			//rgn.CreatePolyPolygonRgn(pts, counts, polygon.NumParts, WINDING);
			//CBrush fillBrush;
			//fillBrush.CreateSolidBrush(RGB(0, 255, 0));
			//pDC->FillRgn(&rgn, &fillBrush);
			//fillBrush.DeleteObject();

			delete counts;
			delete[] pts;

			for (int i = 0; i<polygon.NumParts; ++i)
			{
				int count = 0;
				const shp::Point* pts = polygon.getPart(i, &count);
				if (count > 0)
				{
					POINT pt;
					this->GeoToMap(pts[0].X, pts[0].Y, &pt);
					pDC->MoveTo(pt.x, pt.y);
					for (int j = 0; j<count; ++j)
					{
						this->GeoToMap(pts[j].X, pts[j].Y, &pt);
						pDC->LineTo(pt.x, pt.y);
					}
				}
			}
		}
			break;
		}
	}

	pDC->SelectObject(OldBrush);
	NewBrush.DeleteObject();
}
*/

void CLDFProcessDlg::OnBUTBrowser() 
{
	// TODO: Add your control notification handler code here
    //��ȡ�����ļ��ļ�·��


	//��һ��
	/*
    wchar_t* allPathName = new wchar_t[MAX_PATH];                 //�������������·����
    GetModuleFileName(NULL,allPathName,MAX_PATH);               //�õ����������·����
    wchar_t *exeName;                                                                                            
    exeName = wcsrchr(allPathName, L'\\');      //�����һ����\�����Ͽ����õ����ĳ�������
    if (exeName != NULL)                                                  //�ѳ������ÿգ���ȥ��
    {
            *exeName = NULL;
    }
    wcscat(allPathName,L"\\Config\\DImageProcess.exe");      //���һ����\\��Ҳ������\\..\\�Լ���Ҫ����ӵ��ļ�·��        
	//sizeof(fName)

	ShellExecute(NULL,L"open",allPathName,NULL,NULL,SW_SHOWNORMAL); 
	delete allPathName; 
	*/
	

	//������http://blog.csdn.net/clever101/article/details/2351113
	/*
	wchar_t* szAppName=new wchar_t[MAX_PATH];
	::GetModuleFileName(NULL, szAppName, MAX_PATH);
	CString str_szAppName;
	str_szAppName.Format(_T("%s"), szAppName);
	CString strTempPath = _T("");
	strTempPath = str_szAppName.Left(str_szAppName.ReverseFind('\\')+1)+_T("Config\\DImageProcess.exe");
	ShellExecute(NULL,L"open",strTempPath,NULL,NULL,SW_SHOWNORMAL); 
	delete szAppName; 
	 */


	//������http://zhidao.baidu.com/link?url=_YJh0Gznu36GbxooyXd0dLgiWRTsSyBf_rwB3OBylhDhlzV2atF8iWz3X6qmgRt0fRvA5Cl28eFxgw4WDfEcVa
	CString str_szAppName;
	::GetModuleFileName(NULL, str_szAppName.GetBuffer(MAX_PATH), MAX_PATH);
	//�ڶ�GetBuffer���ص�ָ��ʹ��֮����Ҫ����ReleaseBuffer����������ʹ������Cstring��operations������ᷢ������.
	//�ҵ������ReleaseBuffer()�����ǽ�CString�ÿ�
	str_szAppName.ReleaseBuffer();//str_szAppName.ReleaseBuffer(MAX_PATH);
	CString strTempPath = _T("");
	strTempPath = str_szAppName.Left(str_szAppName.ReverseFind('\\')+1)+_T("Config\\DImageProcess.exe");
	ShellExecute(NULL,L"open",strTempPath,NULL,NULL,SW_SHOWNORMAL); 
}

void CLDFProcessDlg::OnBUTInput() 
{
	// TODO: Add your control notification handler code here
	CInputDlg t_InputDlg;
	t_InputDlg.DoModal();
}

void CLDFProcessDlg::OnBUTCLOUDetection() 
{
	// TODO: Add your control notification handler code here
	CCldetcDlg t_CldetcDlg;
	t_CldetcDlg.DoModal();
}

void CLDFProcessDlg::OnBUTAbnormal() 
{
	// TODO: Add your control notification handler code here
	CAbnmdlDlg t_AbnmdlDlg;
	t_AbnmdlDlg.DoModal();
}

void CLDFProcessDlg::OnClickedButLegend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CLegendDlg t_LegendDlg;
	t_LegendDlg.DoModal();
}

void CLDFProcessDlg::OnBUTOutput() 
{
	// TODO: Add your control notification handler code here
	COutputDlg t_OutputDlg;
	t_OutputDlg.DoModal();
}

CString generateOutFilename(CString filepath, CString filename,CString fileextension, CString prefix)
{
	CString str, rst;
	for (int ist = filename.GetLength() - 1; ist > 0; ist--){
		if (filename.Mid(ist, 1) == "\\"){
			str = filename.Right(filename.GetLength() - ist - 1);
			break;
		}
	}
	str += fileextension;
	rst = filepath;
	rst += "\\";
	rst += prefix;
	rst += str;
	return rst;
}

void CLDFProcessDlg::OnButCalculate() 
{
	// TODO: Add your control notification handler code here

	if (m_strArray.GetSize() < 1){
		AfxMessageBox(_T("�ļ����㣡"),MB_OK|MB_ICONERROR);
		return;
	}
	else if (m_outputfolder.GetLength() < 1)//ע�⣬���GetConSetting()�в�ʹ��m_outputfolder.ReleaseBuffer();GetLength()�᲻��ȷ
	{
		AfxMessageBox(_T("��ָ������ļ�·��"),MB_OK|MB_ICONERROR);
		return;
	}
	else
	{
		//�����û������̣߳����ڽ��ȵ���ʾ
		pUIThread=AfxBeginThread(RUNTIME_CLASS(CprocessUIThread));
		if (pUIThread == NULL)
		{
			AfxMessageBox(_T("�û������߳�����ʧ��!"),MB_OK|MB_ICONERROR);
			return;
		}
		//���ݲ�������ת�����������
		//pUIThread->PostThreadMessage(WM_THREADINFO,0,(LPARAM)m_strArray[0].GetBuffer(0));
		//���������̣߳������ļ��ļ��㴦��
		pThread=AfxBeginThread(ThreadFunc,this);
		if (pThread == NULL)
		{
			AfxMessageBox(_T("�����߳�����ʧ��!"),MB_OK|MB_ICONERROR);
			return;
		}
		//SetTimer(1,1000,NULL);//�ٶ�ͳ��
		SetTimer(2,100,NULL);//������ʱ
		//m_stTip.SetWindowText("������...");
		pUIThread->PostThreadMessage(WM_THREADINFO,2,1);//����
	}
}

void CLDFProcessDlg::statCircle(double m_dCenterLon,double m_dCenterLat,double m_dRadius)
{
	LDF ldf0;
	ldf0.readHdr(m_strArray[0]);
	double m_xres = ldf0.m_hdr.fLonSolution;
	double m_yres = ldf0.m_hdr.fLatSolution;
	double m_minX = ldf0.m_hdr.fMinLon;
	double m_maxX = ldf0.m_hdr.fMaxLon;
	double m_minY = ldf0.m_hdr.fMinLat;
	double m_maxY = ldf0.m_hdr.fMaxLat;

	if(m_dCenterLon - m_dRadius < m_minX || m_dCenterLon + m_dRadius > m_maxX){
		MessageBox(_T("ָ����Χ��Ч"));
		return;
	}
	if(m_dCenterLat - m_dRadius < m_minY || m_dCenterLat + m_dRadius > m_maxY){
		MessageBox(_T("ָ����Χ��Ч"));
		return;
	}
	if(m_dRadius <= 0){
		MessageBox(_T("ָ���뾶��Ч"));
		return;
	}

	//�������Ļ������ο���http://blog.sina.com.cn/s/blog_671892cf01018sxv.html
	std::locale &loc=std::locale::global(std::locale(std::locale(),"",LC_CTYPE)); //���������䣬�޷�����д��
	std::wofstream outfile;
	CString fName;
	fName.Format(_T("%.1fE%.1fN%s"),m_dCenterLon,m_dCenterLat,_T(".txt"));
	outfile.open(m_outputfolder+_T("\\statistics\\")+fName);
	//���û�ԭ���Ļ���
	std::locale::global(loc);//���������䣬�޷�����д��

	statis st;
	CString str = _T("�ļ���\tƽ��ֵ\t���ֵ\t��Сֵ\t����\t��׼��");
	outfile << str.GetString() << std::endl;


	for (size_t i = 0; i < m_strArray.GetSize(); i++)
	{
		LDF ldf1;
		ldf1.read(m_strArray[i]);
		st = statAvg(ldf1, m_dCenterLon, m_dCenterLat, m_dRadius, m_Channel, m_thrld_Month[ldf1.m_hdr.wMonth], 350);
		
		outfile << m_strArray[i].GetString() << "\t" << st.avg
			<< "\t" << st.max << "\t" << st.min
			<< "\t" << st.var << "\t" << st.dev;
		outfile << std::endl;
	}
	outfile.close();
	return;
}

void CLDFProcessDlg::statRect(double m_dCenterLon,double m_dCenterLat,double m_dRadius_Width,double m_dRadius_Height)
{
	LDF ldf0;
	ldf0.readHdr(m_strArray[0]);
	double m_xres = ldf0.m_hdr.fLonSolution;
	double m_yres = ldf0.m_hdr.fLatSolution;
	double m_minX = ldf0.m_hdr.fMinLon;
	double m_maxX = ldf0.m_hdr.fMaxLon;
	double m_minY = ldf0.m_hdr.fMinLat;
	double m_maxY = ldf0.m_hdr.fMaxLat;

	double minLon = m_dCenterLon - m_dRadius_Width;
	double maxLon = m_dCenterLon + m_dRadius_Width;
	double minLat = m_dCenterLat - m_dRadius_Height;
	double maxLat = m_dCenterLat + m_dRadius_Height;

	if(minLon < m_minX || maxLon > m_maxX){
		MessageBox(_T("ָ����Χ��Ч"));
		return;
	}

	if(minLat < m_minY || maxLat > m_maxY){
		MessageBox(_T("ָ����Χ��Ч"));
		return;
	}

	//�������Ļ������ο���http://blog.sina.com.cn/s/blog_671892cf01018sxv.html
	std::locale &loc=std::locale::global(std::locale(std::locale(),"",LC_CTYPE)); //���������䣬�޷�����д��
	std::wofstream outfile;
	CString fName;
	fName.Format(_T("%.1fE%.1fN%s"),m_dCenterLon,m_dCenterLat,_T(".txt"));
	outfile.open(m_outputfolder+_T("\\statistics\\")+fName);
	//���û�ԭ���Ļ���
	std::locale::global(loc);//���������䣬�޷�����д��

	statis st;
	CString str = _T("�ļ���\tƽ��ֵ\t���ֵ\t��Сֵ\t����\t��׼��");
	outfile << str.GetString() << std::endl;


	for (size_t i = 0; i < m_strArray.GetSize(); i++)
	{
		LDF ldf1;
		ldf1.read(m_strArray[i]);
		st = statAvg(ldf1, minLon, maxLon, minLat, maxLat, m_Channel, m_thrld_Month[ldf1.m_hdr.wMonth], 350);
		outfile << m_strArray[i].GetString() << "\t" << st.avg
			<< "\t" << st.max << "\t" << st.min
			<< "\t" << st.var << "\t" << st.dev;
		outfile << std::endl;
	}
	outfile.close();
	return;
}

void CLDFProcessDlg::PolyLineToPoints(IProvider * pProvider, 
	std::vector<int>& cts, 
	std::vector<pointStruct>& points)
{
	cts.clear();
	points.clear();
	std::vector<const shp::ShapeObject *> vecShp = pProvider->getGeometries();
	for ( IProvider::ITER_SHP iter = vecShp.begin();
		iter < vecShp.end();
		iter++ )
	{
		const shp::ShapeObject * pShp = *iter;
		if(pShp->ShapeType != shp::ShapeType::PolyLine){
			continue;
		}
		const shp::ShpPolyLine& polyLine = *static_cast<const shp::ShpPolyLine *>(pShp);
		
		int * counts = new int[polyLine.NumParts];

		for ( int i=0; i<polyLine.NumParts - 1; ++i ){
			counts[i] = polyLine.Parts[i+1] - polyLine.Parts[i];
			cts.push_back(polyLine.Parts[i+1] - polyLine.Parts[i]);
		}
		counts[polyLine.NumParts - 1] = polyLine.NumPoints - polyLine.Parts[polyLine.NumParts - 1];
		cts.push_back(polyLine.NumPoints - polyLine.Parts[polyLine.NumParts - 1]);
		
		//���⿴����shp��ÿ���߶��ŵ���˳�򶼷ŵ�points���ˣ��Ǹ�bug
		//�ں���Ĵ����У����һ��ʸ����ĩ�˺���һ��ʸ�����׶�֮������ߵ������Ѵ�ͳ������
		for (int i=0; i<polyLine.NumParts; i++){
			int count = 0;
			const shp::Point* pts = polyLine.getPart(i, &count);
			if ( count > 0 ){
				pointStruct pt;
				for (int j=0; j<count; ++j){
					pt.x = pts[j].X;
					pt.y = pts[j].Y;
					points.push_back(pt);
				}
			}
		}
		delete counts;
	}

}

//���ص����ڲ������ͳ�ƽ����statis& baseST����Χ�����ͳ�ƽ��
statis CLDFProcessDlg::statSinglePolyLine(LDF& ldf, std::vector<pointStruct>&pts,
	double minDis, double maxDis, int nBand, double min, double max, statis& baseST)
{
	statis st;
	st.min = max;
	st.max = min;
	st.avg = 0;
	st.dev = 0;
	st.var = 0;
	size_t sample = ldf.m_hdr.wWidth;
	size_t line = ldf.m_hdr.wHeight;

	std::vector<short> faultVal;  //���Ѵ��ڲ�����
	std::vector<short> baseVal;  //���Ѵ���Χ����

	for(size_t iy = 0; iy < line; iy++){
		for(size_t ix = 0; ix < sample; ix++){
			pointStruct Pa;
			ldf.pix2geo(ix, iy, Pa.x, Pa.y);
			double dis = getNearestDistance(pts, Pa);
			short val = ldf.getPixel(nBand, ix, iy);
			if(val <= max * 10 && val >= min * 10){
				if(dis <= minDis){
					faultVal.push_back(val);
				}
				if(dis > minDis && dis <= maxDis){
					baseVal.push_back(val);
				}
			}
		}
	}
	
	baseST = stat(baseVal);
	st = stat(faultVal);
	return st;
}

void CLDFProcessDlg::statPolyLine(std::vector<int>& cts, 
	std::vector<pointStruct>& points, double minDis,
	double maxDis, int nBand, double min, double max)
{
	//�������Ļ������ο���http://blog.sina.com.cn/s/blog_671892cf01018sxv.html
	std::locale &loc=std::locale::global(std::locale(std::locale(),"",LC_CTYPE)); //���������䣬�޷�����д��
	std::wofstream outfile;
	CString fName;
	fName.Format(_T("���Ѵ�����ͳ��.txt"));
	outfile.open(m_outputfolder+_T("\\statistics\\")+fName);
	//���û�ԭ���Ļ���
	std::locale::global(loc);//���������䣬�޷�����д��
	
	CString str = _T("�ļ���\tƽ��ֵ\t���ֵ\t��Сֵ\t����\t��׼��\t����ƽ��ֵ\t�������ֵ\t������Сֵ\t��������\t������׼��");
	outfile << str.GetString() << std::endl;

	statis st;
	std::vector<pointStruct> spts;
	for(int i = 0; i < cts[0]; i++){
		spts.push_back(points[i]);
	}

	for (size_t i = 0; i < m_strArray.GetSize(); i++)
	{
		LDF ldf1;
		ldf1.read(m_strArray[i]);
		std::vector<CPoint> pts;
		//stf���ڲ������ͳ�ƽ����stb����Χ�����ͳ�ƽ��
		statis stf, stb;
		//�����޸ģ���������int nBand, double min��û����
		stf = statSinglePolyLine(ldf1, spts, minDis, maxDis, m_Channel, m_thrld_Month[ldf1.m_hdr.wMonth], max, stb);//nBand, min, max, stb);

		outfile << m_strArray[i].GetString() << "\t" << stf.avg
			<< "\t" << stf.max << "\t" << stf.min
			<< "\t" << stf.var << "\t" << stf.dev
			<< "\t" << stb.avg << "\t" << stb.max
			<< "\t" << stb.min << "\t" << stb.var
			<< "\t" << stb.dev;
		outfile << std::endl;
	}
	outfile.close();
	return;
}


void CLDFProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1)
	{
		//pUIThread->PostThreadMessage(WM_THREADINFO, 4, m_nSpeed);
		//m_nSpeed = 0;
	}
	else if(nIDEvent == 2)
	{
		m_fTime += 0.1;
		pUIThread->PostThreadMessage(WM_THREADINFO, 5, (LPARAM)&m_fTime);
	}
	
	CDialog::OnTimer(nIDEvent);
}


//�ļ����㴦�����
UINT ThreadFunc(LPVOID pParm)
{
	CLDFProcessDlg *pInfo=(CLDFProcessDlg*)pParm;

	//�õ��ļ�����Ŀ�����ڼ������
	DWORD dwTotalSize = (pInfo->m_strArray).GetSize();
	DWORD dwCompleteSize = 0;//����ɵ���Ŀ
	//����
	DWORD dwStep = 1;
	//�����׸��������ļ����ƣ����������������Ի�����Ϊ�Ի����п��ܻ�û����
	pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,0,(LPARAM)(pInfo->m_strArray)[0].GetBuffer(0));

	//������BOOL CLegendDlg::draw_picture_showlegend()
	//����m_Shape_strInfo
	COLORREF shp_color_epicenter,shp_color_provinborder,shp_color_fault;
	CString shp_strInfo=pInfo->m_Shape_strInfo , str_tmp;
	int shp_size_epicenter, shp_size_provinborder, shp_size_fault;

	AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,0,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_epicenter,&shp_size_epicenter); 

	AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,1,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_provinborder,&shp_size_provinborder); 
	
	AfxExtractSubString(str_tmp,(LPCTSTR)shp_strInfo,2,'|');
	swscanf_s(str_tmp.GetBuffer(0),_T("%8x%d"),&shp_color_fault,&shp_size_fault); 
	//����m_Shape_strInfo���

	if(pInfo->m_CHEK_Statistics){
		// �ж�·���Ƿ����
		if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\statistics")))
		{
			if (!CreateDirectory(pInfo->m_outputfolder+_T("\\statistics"), NULL ) )
			{return 0;}
		}
		//*************************���ͳ��***************************



		if ( 0 == pInfo->m_StasticArea_SelectStyle || 1 == pInfo->m_StasticArea_SelectStyle )//����Բ�λ����ͳ������
		{
			//����m_StasticArea_strInfo//��CLegendDlg::OnInitDialog()�޸�
			int nItem = 0 ;
			CString str_tmp;
			str_tmp= pInfo->m_StasticArea_strInfo.Left(pInfo->m_StasticArea_strInfo.Find('|'));
			swscanf_s(str_tmp.GetBuffer(0),_T("%d"),&nItem);
			float Lon,Lat,Width,Height;
			for(int i=0; i<nItem; i++)
			{
				AfxExtractSubString(str_tmp,(LPCTSTR)(pInfo->m_StasticArea_strInfo),i+1,'|');//i+1������4��һ����ָ�ĸ���

				swscanf_s(str_tmp.GetBuffer(0),_T("%f%f%f%f"),&Lon,&Lat,&Width,&Height); 
				str_tmp.Format(_T("%.2f"),Lon);  
				str_tmp.Format(_T("%.2f"),Lat); 
				str_tmp.Format(_T("%.2f"),Width);
				str_tmp.Format(_T("%.2f"),Height);
				if ( 0 == pInfo->m_StasticArea_SelectStyle)
					pInfo->statCircle(Lon,Lat,Width);//Բ��ͳ��
				else
					pInfo->statRect(Lon,Lat,Width,Height);//����ͳ��
			}
			//����m_StasticArea_strInfo���
		}
		else//���Ƕ���ͳ������
		{
			pInfo->OpenShape(pInfo->GetAppPath() + _T("Config\\fault_statistic.shp"));
			std::vector<int>cts;
			std::vector<pointStruct>points;
			pInfo->PolyLineToPoints(pInfo->m_pProvider, cts, points);
			double minDis = 25 * 0.0083;//ħ�������Ѵ��ڲ���25�ĵ�λ������
			double maxDis = 50 * 0.0083;//ħ�������Ѵ���Χ��50�ĵ�λ������
			//�����޸�statPolyLine������ȥ��Info->m_Channel, 200��û����
			pInfo->statPolyLine(cts, points, minDis, maxDis, pInfo->m_Channel, 200, 350);
		}


		

		//MessageBox(_T("������ɣ�"));
		/****************************************************************/
	}




	if(pInfo->m_CHEK_Compound){
		// �ж�·���Ƿ����
		if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\Compound")))
		{
			if (!CreateDirectory(pInfo->m_outputfolder+_T("\\Compound"), NULL ) )
			{return 0;}
		}

		//*************************���ƺϳ�***************************
		//��ʼ���������ļ������������ٷֱ�
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,6,(LPARAM)(pInfo->m_strArray)[0].GetBuffer(0));
		//�ض���
		dwTotalSize = (pInfo->m_strArray).GetSize();
		dwCompleteSize = 0;

		LDF ldf1;
		ldf1.read((pInfo->m_strArray)[0]);

		//�Ѷ����һ���ļ������½���
		dwCompleteSize += dwStep;
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));

		for (size_t i = 1; i < (pInfo->m_strArray).GetSize(); i++)
		{
			//���ݴ������ļ�����
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,4,(LPARAM)(pInfo->m_strArray[i]).GetBuffer(0));
			LDF ldf2;
			ldf2.read((pInfo->m_strArray)[i]);
			noCloudLdfBlend(ldf1, ldf2, (pInfo->m_thrld_Month)[ldf2.m_hdr.wMonth]*10, (pInfo->m_thrld_Month)[0]*10);

			//���½���
			dwCompleteSize += dwStep;
			//pInfo->m_nSpeed += dwStep;
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));
		}

		ldf1.write(pInfo->m_outputfolder+_T("\\Compound\\Compound.ldf"));
		//MessageBox(_T("������ɣ�"));
		/****************************************************************/
	}

	if(pInfo->m_CHEK_Pseudocolor){
		// �ж�·���Ƿ����
		if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\color")))
		{
			if (!CreateDirectory(pInfo->m_outputfolder+_T("\\color"), NULL ) )
			{return 0;}
		}
		//*************************α��ɫ�ϳ�***************************
		std::vector<CString> aShpnames;//ħ��
		aShpnames.resize(3);
		aShpnames[0] = pInfo->GetAppPath() + _T("Config\\provinborder.shp");//ʡ��
		aShpnames[1] = pInfo->GetAppPath() + _T("Config\\fault.shp");//�ϲ�
		aShpnames[2] = pInfo->GetAppPath() + _T("Config\\epicenter.shp");//����
		//��ʼ���������ļ������������ٷֱ�
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,6,(LPARAM)(pInfo->m_strArray)[0].GetBuffer(0));
		//�ض���
		dwTotalSize = (pInfo->m_strArray).GetSize();
		dwCompleteSize = 0;

		CString outpath;
		outpath = pInfo->m_outputfolder;

		for (size_t i = 0; i < (pInfo->m_strArray).GetSize(); i++){
			//���ݴ������ļ�����
			if(i!=0)
				pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,4,(LPARAM)(pInfo->m_strArray[i]).GetBuffer(0));

			(pInfo->m_ldf).read((pInfo->m_strArray)[i]);
			(pInfo->m_pLPic).setRange(pInfo->m_Channel,pInfo->m_dmin,pInfo->m_dmax,pInfo->m_fontsize_legend);
			(pInfo->m_pLPic).setLDF(pInfo->m_ldf);

			for (size_t j = 0; j < aShpnames.size(); j++){
				pInfo->OpenShape(aShpnames[j]);
				if (pInfo->m_pProvider){
					if (j==0 && pInfo->m_CHECK_provin_border){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_provinborder),GetGValue(shp_color_provinborder),GetBValue(shp_color_provinborder)),shp_size_provinborder,1);}//ʡ��
					if (j==1 && pInfo->m_CHECK_fault){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_fault),GetGValue(shp_color_fault),GetBValue(shp_color_fault)),shp_size_fault,1);}//�ϲ�
					if (j==2 && pInfo->m_CHECK_epicenter){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_epicenter),GetGValue(shp_color_epicenter),GetBValue(shp_color_epicenter)),shp_size_epicenter,2);}//����
				}
			}
			(pInfo->m_pLPic).drawedge(pInfo->m_ldf);

			CString outfile = generateOutFilename(outpath+_T("\\color"), (pInfo->m_strArray)[i],_T(".png"),_T("color_"));
			(pInfo->m_pLPic).Save(outfile);
			//���½���
			dwCompleteSize += dwStep;
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));
		}
		//MessageBox(_T("�������"));
		/****************************************************************/
	}

	if((!pInfo->m_selectoutputstyle) && (0 == pInfo->m_algorithm)){
		// �ж�·���Ƿ����
		if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\bkgnd")))
		{
			if (!CreateDirectory(pInfo->m_outputfolder+_T("\\bkgnd"), NULL ) )
			{return 0;}
		}
		//*************************Kֵ����������***************************
		//��ʼ���������ļ������������ٷֱ�
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,6,(LPARAM)(pInfo->m_strArray)[0].GetBuffer(0));
		//�ض���
		dwTotalSize = (pInfo->m_strArray).GetSize();
		dwCompleteSize = 0;

		const size_t filecnt = (pInfo->m_strArray).GetSize();
		LDF *ldf=new LDF[filecnt];

		for (size_t i = 0; i < filecnt; i++)
		{
			//���ݴ������ļ�����
			if(i!=0)
				pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,4,(LPARAM)(pInfo->m_strArray[i]).GetBuffer(0));

			ldf[i].read((pInfo->m_strArray)[i]);

			//���½���
			dwCompleteSize += dwStep;
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));
		}

		size_t nImgWidth = ldf[0].m_hdr.wWidth;
		size_t nImgHeight = ldf[0].m_hdr.wHeight;
		float *Mean =new float[nImgWidth*nImgHeight]();
		float *Sigma =new float[nImgWidth*nImgHeight]();

		CString str_bkgnd;
		str_bkgnd.Format(_T("��%d��"),0);
		//��ʼ���������ļ������������ٷֱ�
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,6,(LPARAM)str_bkgnd.GetBuffer(0));
		//�ض���
		dwTotalSize = nImgHeight;
		dwCompleteSize = 0;


		for(size_t iy = 0; iy < nImgHeight; iy++){
			//���ݴ������ļ�����
			if(iy!=0){
				str_bkgnd.Format(_T("��%d��"),iy);
				pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,4,(LPARAM)str_bkgnd.GetBuffer(0));
			}
			for(size_t ix = 0; ix < nImgWidth; ix++){
				double gx = 0, gy = 0;
				//�õ���һ��ͼ��ÿ��ľ�γ��
				ldf[0].pix2geo(ix, iy, gx, gy);
				//�õ�ÿ��ͼ���Ӧ��γ�ȵ�����ֵ����͡�����
				double *v=new double[filecnt]();//�洢ĳ���ʱ�����У��������Ϊ0��
				int cnt = 0;//�洢ĳ���ʱ�����е�������ֻͳ���������ݣ�
				double sum = 0;//�洢ĳ���ʱ�����еĺͣ�ֻͳ���������ݣ�
				for(size_t i = 0; i < filecnt; i++){
					v[i] = ldf[i].getPixelGeo(pInfo->m_Channel, gx, gy)/10.0;
					if( v[i] > (pInfo->m_thrld_Month)[ldf[i].m_hdr.wMonth] && v[i] < (pInfo->m_thrld_Month)[0]*10){
						sum += v[i];
						cnt ++;
					}
					else{v[i]=0;}
				}
				//�õ���ÿ��ͼ���в�ȫ����
				if(0 != cnt){
					Mean[iy*nImgWidth+ix]  =  sum /cnt;
					sum = 0;//�ù�������ʱ����
					for(size_t i = 0; i < filecnt; i++){
						if(v[i]!=0)
						sum += (v[i] - Mean[iy*nImgWidth+ix]) * (v[i] -Mean[iy*nImgWidth+ix]);
					}
					Sigma[iy*nImgWidth+ix] = (float)sqrt(sum /cnt);
				//�õ���ÿ��ͼ����ȫ���ƣ�����ͳ��
				}else{
					Mean[iy*nImgWidth+ix]  = 0;
					Sigma[iy*nImgWidth+ix] = 0;
				}

///////////////////////////////���㷨***********************************
				sum=0;//�ù�������ʱ����
				if(0 != cnt){
					for(size_t i = 0; i < filecnt; i++){
						if(v[i]!=0){
							if((Mean[iy*nImgWidth+ix] - v[i])/2>Sigma[iy*nImgWidth+ix])
							{
								v[i]=0;
								cnt--;
							}
							else
							{sum+=v[i];}
						}
					}
				}
///////////////////////////////���㷨***********************************
				if(0 != cnt){
					Mean[iy*nImgWidth+ix]  =  sum /cnt;
					sum = 0;//�ù�������ʱ����
					for(size_t i = 0; i < filecnt; i++){
						if(v[i]!=0)
						sum += (v[i] - Mean[iy*nImgWidth+ix]) * (v[i] -Mean[iy*nImgWidth+ix]);
					}
					Sigma[iy*nImgWidth+ix] = (float)sqrt(sum /cnt);
				//�õ���ÿ��ͼ����ȫ���ƣ�����ͳ��
				}else{
					Mean[iy*nImgWidth+ix]  = 0;
					Sigma[iy*nImgWidth+ix] = 0;
				}
				delete [] v;
			}
			//���½���
			dwCompleteSize += dwStep;
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));
		}




		//*************************α��ɫ�ϳ�***************************
		std::vector<CString> bShpnames;//ħ��
		bShpnames.resize(3);
		bShpnames[0] = pInfo->GetAppPath() + _T("Config\\provinborder.shp");//ʡ��
		bShpnames[1] = pInfo->GetAppPath() + _T("Config\\fault.shp");//�ϲ�
		bShpnames[2] = pInfo->GetAppPath() + _T("Config\\epicenter.shp");//����

		CString outfile_bkgnd_color;
		outfile_bkgnd_color.Format(_T("%s%s%d%s"),pInfo->m_outputfolder,_T("\\bkgnd\\KColor_MeanSigma"),ldf[0].m_hdr.wMonth,_T(".png"));

		//��Ϊ��ȡldfͷ�ļ���Ϣ
		(pInfo->m_ldf).read((pInfo->m_strArray)[0]);
		(pInfo->m_pLPic).setRange(pInfo->m_Channel,(pInfo->m_dmin)/10,(pInfo->m_dmax)/10,pInfo->m_fontsize_legend);
		(pInfo->m_pLPic).setTIFF(pInfo->m_ldf,Mean);

		for (size_t j = 0; j < bShpnames.size(); j++){
			pInfo->OpenShape(bShpnames[j]);
			if (pInfo->m_pProvider){
				if (j==0 && pInfo->m_CHECK_provin_border){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_provinborder),GetGValue(shp_color_provinborder),GetBValue(shp_color_provinborder)),shp_size_provinborder,1);}//ʡ��
				if (j==1 && pInfo->m_CHECK_fault){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_fault),GetGValue(shp_color_fault),GetBValue(shp_color_fault)),shp_size_fault,1);}//�ϲ�
				if (j==2 && pInfo->m_CHECK_epicenter){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_epicenter),GetGValue(shp_color_epicenter),GetBValue(shp_color_epicenter)),shp_size_epicenter,2);}//����
			}
		}
		(pInfo->m_pLPic).drawedge(pInfo->m_ldf);

		(pInfo->m_pLPic).Save(outfile_bkgnd_color);

		//MessageBox(_T("�������"));
		/****************************************************************/




		//׼������ͼƬ
		//char* szFileName = "D:\\aTemp\\ENOAA18_2011_04_01_19_00_JLJ_KVal.tif";
		GDALAllRegister(); //ע����������
		GDALDriver *poDriver=GetGDALDriverManager()->GetDriverByName("GTiff");

		double GeoTrans_out[6];
		GeoTrans_out[0] = ldf[0].m_hdr.fMinLon; // ���Ͻ� x //
		GeoTrans_out[1] = ldf[0].m_hdr.fLonSolution; // ��������һ�����ض�Ӧ�ľ��� //
		GeoTrans_out[2] = 0; // ��ת, 0��ʾ����Ϊ���� //
		GeoTrans_out[3] = ldf[0].m_hdr.fMaxLat; // ���Ͻ� y //
		GeoTrans_out[4] = 0; // ��ת, 0��ʾ����Ϊ���� //
		GeoTrans_out[5] = -ldf[0].m_hdr.fLatSolution; // �ϱ�����һ�����ض�Ӧ�ľ��� //

		int nBandCount_bkgnd=2;

		char *papszOptions;
		papszOptions = NULL;//"INTERLEAVE=PIXEL";

		CString outfile_bkgnd;
		outfile_bkgnd.Format(_T("%s%s%d%s"),pInfo->m_outputfolder,_T("\\bkgnd\\MeanSigma"),ldf[0].m_hdr.wMonth,_T(".tif"));
		const char *outfile_bkgnd2;
		//CStringתconst char* 
		USES_CONVERSION;
		outfile_bkgnd2 = T2A(outfile_bkgnd);

		//////////////////////////////////
		delete [] ldf;
		//////////////////////////////////

		GDALDataset *poDstDS = poDriver->Create(outfile_bkgnd2, nImgWidth,nImgHeight,nBandCount_bkgnd,GDT_Float32,&papszOptions);

		poDstDS->SetGeoTransform(GeoTrans_out);

		//poDstDS->RasterIO(GF_Write,0,0,nImgWidth,nImgHeight,img,nImgWidth,nImgHeight,GDT_Float32,nBandCount_bkgnd,NULL,0,0,0);
		poDstDS->GetRasterBand(1)->RasterIO(GF_Write,0,0,nImgWidth,nImgHeight,Mean,nImgWidth,nImgHeight,GDT_Float32,0,0);
		poDstDS->GetRasterBand(2)->RasterIO(GF_Write,0,0,nImgWidth,nImgHeight,Sigma,nImgWidth,nImgHeight,GDT_Float32,0,0);
		//poDstDS->GetRasterBand(3)->RasterIO(GF_Write,0,0,nImgWidth,nImgHeight,Sigma,nImgWidth,nImgHeight,GDT_Float32,0,0);
		delete poDstDS;
		poDstDS=NULL;


		delete [] Mean;
		delete [] Sigma;
		//MessageBox(_T("�������"));
		/****************************************************************/
	}


	if((pInfo->m_CHEK_Kpseudocolor) && (0 == pInfo->m_algorithm)){
		// �ж�·���Ƿ����
		if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\KValue")))
		{
			if (!CreateDirectory(pInfo->m_outputfolder+_T("\\KValue"), NULL ) )
			{return 0;}
		}

		//*************************Kֵ����+α��ɫ��ͼ**************************
		//��ʼ���������ļ������������ٷֱ�
		pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,6,(LPARAM)(pInfo->m_strArray)[0].GetBuffer(0));
		//�ض���
		dwTotalSize = (pInfo->m_strArray).GetSize();
		dwCompleteSize = 0;

		if ((pInfo->m_bkgndfile).IsEmpty()){
			AfxMessageBox(_T("��ѡ�񱳾����ļ���"),MB_OK|MB_ICONERROR);
			return 0;
		}

		USES_CONVERSION;
		char* szFileName = T2A(pInfo->m_bkgndfile);
		GDALDataset *poDataset;   //GDAL���ݼ�
		GDALAllRegister(); //ע����������

		poDataset = (GDALDataset*)GDALOpen(szFileName,GA_ReadOnly);
		if( poDataset == NULL )
		{
			AfxMessageBox(_T("�ļ���ʧ��!!!"));
			return 0;
		} 

		//��ȡͼ�񲨶�
		GDALRasterBand *poBand1;   //ң�е�һ������
		GDALRasterBand *poBand2;

		int nBandCount_input = poDataset->GetRasterCount();
		poBand1 = poDataset->GetRasterBand(1);
		poBand2 = poDataset->GetRasterBand(2);


		//��ȡ����任ϵ��
		double GeoTrans_in[6];
		poDataset->GetGeoTransform( GeoTrans_in );

		int nImgSizeX_in = poDataset->GetRasterXSize();
		int nImgSizeY_in = poDataset->GetRasterYSize();
		size_t nImgWidth_in = nImgSizeX_in;
		size_t nImgHeight_in = nImgSizeY_in;

		//Ϊ���ݻ����������ڴ�
		float *Mean1,*sigma1;
		Mean1 = (float *) CPLMalloc((nImgWidth_in)*(nImgHeight_in)*sizeof(float));
		sigma1 = (float *) CPLMalloc((nImgWidth_in)*(nImgHeight_in)*sizeof(float));


		//��ȡ����
		poBand1->RasterIO( GF_Read, 0, 0,nImgSizeX_in,nImgSizeY_in, 
			Mean1,nImgSizeX_in,nImgSizeY_in, GDT_Float32,0, 0 );
		poBand2->RasterIO( GF_Read, 0, 0,nImgSizeX_in,nImgSizeY_in, 
			sigma1,nImgSizeX_in,nImgSizeY_in, GDT_Float32,0, 0 );

		for (size_t i = 0; i < (pInfo->m_strArray).GetSize() ; i++)
		{
			//���ݴ������ļ�����
			if(i!=0)
				pInfo->pUIThread->PostThreadMessage(WM_THREADINFO,4,(LPARAM)(pInfo->m_strArray[i]).GetBuffer(0));
			LDF ldf_KValue;
			ldf_KValue.read((pInfo->m_strArray)[i]);
			float *KValue =new float[nImgWidth_in*nImgHeight_in]();
			for(size_t iy = 0; iy < nImgHeight_in; iy++){
				for(size_t ix = 0; ix < nImgWidth_in; ix++){
					if(0==Mean1[iy*nImgWidth_in+ix] && 0==sigma1[iy*nImgWidth_in+ix])
						KValue[iy*nImgWidth_in+ix]=0;
					else
					{
						double gx = 0, gy = 0;
						//�õ���һ��ͼ��ÿ��ľ�γ��
						gx = GeoTrans_in[0] + ix * GeoTrans_in[1];
						gy = GeoTrans_in[3] + iy * GeoTrans_in[5];
						float v = ldf_KValue.getPixelGeo(pInfo->m_Channel, gx, gy)/10.0;
						if( v > (pInfo->m_thrld_Month)[ldf_KValue.m_hdr.wMonth] && v < (pInfo->m_thrld_Month)[0]*10){
							KValue[iy*nImgWidth_in+ix]=(v-Mean1[iy*nImgWidth_in+ix])/sigma1[iy*nImgWidth_in+ix];
						}
						else{
							KValue[iy*nImgWidth_in+ix]  = 0;
						}
					}
				}		
			}

			
			//׼������ͼƬ
			if(1){
				// �ж�·���Ƿ����
				if (!PathIsDirectory(pInfo->m_outputfolder+_T("\\KColor")))
				{
					if (!CreateDirectory(pInfo->m_outputfolder+_T("\\KColor"), NULL ) )
					{return 0;}
				}
				//*************************α��ɫ�ϳ�***************************
				std::vector<CString> aShpnames;//ħ��
				aShpnames.resize(3);
				aShpnames[0] = pInfo->GetAppPath() + _T("Config\\provinborder.shp");//ʡ��
				aShpnames[1] = pInfo->GetAppPath() + _T("Config\\fault.shp");//�ϲ�
				aShpnames[2] = pInfo->GetAppPath() + _T("Config\\epicenter.shp");//����

				//��Ϊ��ȡldfͷ�ļ���Ϣ
				(pInfo->m_ldf).read((pInfo->m_strArray)[0]);//ħ��
				(pInfo->m_pLPic).setRange(pInfo->m_Channel,pInfo->m_dkmin,pInfo->m_dkmax,pInfo->m_fontsize_legend);
				(pInfo->m_pLPic).setTIFF(pInfo->m_ldf,KValue);

				for (size_t j = 0; j < aShpnames.size(); j++){
					pInfo->OpenShape(aShpnames[j]);
					if (pInfo->m_pProvider){
						if (j==0 && pInfo->m_CHECK_provin_border){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_provinborder),GetGValue(shp_color_provinborder),GetBValue(shp_color_provinborder)),shp_size_provinborder,1);}//ʡ��
						if (j==1 && pInfo->m_CHECK_fault){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_fault),GetGValue(shp_color_fault),GetBValue(shp_color_fault)),shp_size_fault,1);}//�ϲ�
						if (j==2 && pInfo->m_CHECK_epicenter){(pInfo->m_pLPic).DrawMap_arg(pInfo->m_pProvider,Gdiplus::Color(GetRValue(shp_color_epicenter),GetGValue(shp_color_epicenter),GetBValue(shp_color_epicenter)),shp_size_epicenter,2);}//����
					}
				}
				(pInfo->m_pLPic).drawedge(pInfo->m_ldf);

				CString outfile = generateOutFilename(pInfo->m_outputfolder+_T("\\KColor"), (pInfo->m_strArray)[i],_T(".png"),_T("KColor_"));
				(pInfo->m_pLPic).Save(outfile);
				//MessageBox(_T("�������"));
				/****************************************************************/
			}
		

			//׼������ͼƬ
			//char* szFileName = "D:\\aTemp\\ENOAA18_2011_04_01_19_00_JLJ_KVal.tif";
			GDALAllRegister(); //ע����������
			GDALDriver *poDriver=GetGDALDriverManager()->GetDriverByName("GTiff");

			int nBandCount_output=1;

			char *papszOptions;
			papszOptions = NULL;//"INTERLEAVE=PIXEL";


			CString outfile_KValue = generateOutFilename(pInfo->m_outputfolder+_T("\\KValue"), (pInfo->m_strArray)[i],_T(".tif"),_T("KValue_"));
			const char *outfile_KValue2;
			USES_CONVERSION;
			outfile_KValue2 = T2A(outfile_KValue);
			GDALDataset *poDstDS = poDriver->Create(outfile_KValue2, nImgWidth_in,nImgHeight_in,nBandCount_output,GDT_Float32,&papszOptions);

			poDstDS->SetGeoTransform(GeoTrans_in);

			//poDstDS->RasterIO(GF_Write,0,0,nImgWidth_in,nImgHeight_in,img,nImgWidth_in,nImgHeight_in,GDT_Float32,nBandCount_output,NULL,0,0,0);
			poDstDS->GetRasterBand(1)->RasterIO(GF_Write,0,0,nImgWidth_in,nImgHeight_in,KValue,nImgWidth_in,nImgHeight_in,GDT_Float32,0,0);
			//poDstDS->GetRasterBand(2)->RasterIO(GF_Write,0,0,nImgWidth_in,nImgHeight_in,Sigma,nImgWidth_in,nImgHeight_in,GDT_Float32,0,0);
			//poDstDS->GetRasterBand(3)->RasterIO(GF_Write,0,0,nImgWidth_in,nImgHeight_in,Sigma,nImgWidth_in,nImgHeight_in,GDT_Float32,0,0);
			delete poDstDS;
			poDstDS=NULL;
			delete [] KValue;
			//���½���
			dwCompleteSize += dwStep;
			pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 3, (LPARAM) int((dwCompleteSize*1.0/dwTotalSize)*100));
		}


		CPLFree(Mean1);
		CPLFree(sigma1);
		GDALClose(poDataset);
		//MessageBox(_T("�������"));
		/****************************************************************/
	}

	//statCircle();





	//pInfo->KillTimer(1);
	pInfo->KillTimer(2);
	AfxMessageBox(_T("������ɣ�"));
	//�����������Ի������
	pInfo->pUIThread->PostThreadMessage(WM_THREADINFO, 10, 1);


	return 0;
}


void CLDFProcessDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�رմ���ǰ���������ļ�
	SetConSetting();

	/*
	CString strName1,strName2,strTemp; 
	int n1,n2;  
	//strName1=""; 
	strName2=_T(""); 

	n1=strName1.GetLength(); 
	n2=strName2.GetLength(); 
	strTemp.Format(_T("%d,%d"),n1,n2);

	AfxMessageBox(strTemp);
	//::WritePrivateProfileString(_T("StudentInfo"),_T("sName"),strName,_T("c://stud//student.ini"));  */


	CDialog::OnClose();
}

//GetAppPath()��GetConSetting()��SetConSetting()
//�ı����� http://zhidao.baidu.com/link?url=_YJh0Gznu36GbxooyXd0dLgiWRTsSyBf_rwB3OBylhDhlzV2atF8iWz3X6qmgRt0fRvA5Cl28eFxgw4WDfEcVa

//�õ�����ȫ·�����Դ��config.ini�����ȡshp��
CString CLDFProcessDlg::GetAppPath()
{
	CString str_szAppName;
	::GetModuleFileName(NULL, str_szAppName.GetBuffer(MAX_PATH), MAX_PATH);
	str_szAppName.ReleaseBuffer();//str_szAppName.ReleaseBuffer(MAX_PATH);
	CString strAppPath = _T("");
	strAppPath = str_szAppName.Left(str_szAppName.ReverseFind('\\')+1);
	return strAppPath;
}

//�ɳ�����config.ini�ĳ�ʼ������
void CLDFProcessDlg::GetConSetting()
{
	/********************************************************************
	ע�⣺��Ҫ���������Ҫ����ı�����Ҫ�޸����¼����ط���
	1��LDFProcessDlg.h�ж���ñ���
	2��Ŀ��.h�ж���ñ���
	3��LDFProcessDlg.cpp�г�ʼ��ʱ���õ�GetConSetting()
	4��LDFProcessDlg.cpp�г���ر�ʱ�����������SetConSetting()
	5��Ŀ��.cpp�еĹ��캯������ü��ϣ�
	6��Ŀ��.cpp�е�OnInitDialog()���������򽻻�
	7��Ŀ��.cpp�е�OnOK()���������򽻻�
	********************************************************************/

	/*������Ĭ������
	m_selectinputstyle=0;
	m_inputfolder=_T("");
	m_InputWindow_Extend=0;

	m_thrld_Month[0]=350;//upper limit�趨ͼ�����ޣ�ָ�����
	m_thrld_Month[1]=215;
	m_thrld_Month[2]=220;
	m_thrld_Month[3]=225;
	m_thrld_Month[4]=230;
	m_thrld_Month[5]=240;
	m_thrld_Month[6]=245;
	m_thrld_Month[7]=245;
	m_thrld_Month[8]=245;
	m_thrld_Month[9]=240;
	m_thrld_Month[10]=235;
	m_thrld_Month[11]=230;
	m_thrld_Month[12]=225;

	m_algorithm=0;

	m_dmin=2450;
	m_dmax=2910;
	m_dkmin=0.0;
	m_dkmax=2.5;
	m_fontsize_legend=50;
	m_CHECK_epicenter=TRUE;
	m_CHECK_provin_border=TRUE;
	m_CHECK_fault=TRUE;

	m_outputfolder=_T("");
	m_selectoutputstyle=1;
	m_CHEK_Clddetection2=FALSE;
	m_CHEK_Pseudocolor=TRUE;
	m_CHEK_Kpseudocolor=FALSE;
	m_CHEK_Statistics=FALSE;
	m_CHEK_Compound=FALSE;
	m_bkgndfile=_T("");

	*/
	
	CString str_Name,str_Value; 
	CString strIniPath = GetAppPath() + _T("Config\\Config.ini");
	// INPUT
	//CStringתLPWSTR �� str_Value.GetBuffer(MAX_PATH)
	str_Name.Format(_T("m_selectinputstyle"));
	m_selectinputstyle = ::GetPrivateProfileInt(_T("INPUT"),str_Name,0,strIniPath);//Ĭ��Ϊ0���ļ��У�

	str_Name.Format(_T("m_inputfolder"));
	::GetPrivateProfileString(_T("INPUT"),str_Name,_T(""),m_inputfolder.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);

	str_Name.Format(_T("m_InputWindow_Extend"));
	m_InputWindow_Extend = ::GetPrivateProfileInt(_T("INPUT"),str_Name,0,strIniPath);//Ĭ��Ϊ0������չ���봰�ڣ�������ʾ��ͼ��Ϣ��	

	// clouddetection
	double default_set[13]={350,215,220,225,230,240,245,245,245,240,235,230,225};//�Ƽ��Ĭ����ֵ
	for(int i=0;i!=12+1;i++)
	{
		str_Name.Format(_T("m_thrld_Month[%d]"),i);
		CString str_tmp;
		str_tmp.Format(_T("%lf"),default_set[i]);
		::GetPrivateProfileString(_T("clouddetection"),str_Name,str_tmp,str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
		//CStringתdouble
		//���ʹ��_tstof������_tstof��ASCII�����±�����Ϊatof����UNICODE�����±�����Ϊ_wtof
		m_thrld_Month[i] = _tstof(str_Value);
	}

	// abnormalmodel
	str_Name.Format(_T("m_algorithm"));
	m_algorithm = ::GetPrivateProfileInt(_T("abnormalmodel"),str_Name,0,strIniPath);

	// LEGEND
	str_Name.Format(_T("m_dmin"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("2450"),str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_dmin = _tstof(str_Value);

	str_Name.Format(_T("m_dmax"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("2910"),str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_dmax = _tstof(str_Value);

	str_Name.Format(_T("m_dkmin"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("0"),str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_dkmin = _tstof(str_Value);

	str_Name.Format(_T("m_dkmax"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("2.5"),str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_dkmax = _tstof(str_Value);

	str_Name.Format(_T("m_fontsize_legend"));
	m_fontsize_legend = ::GetPrivateProfileInt(_T("LEGEND"),str_Name,50,strIniPath);

	str_Name.Format(_T("m_CHECK_epicenter"));
	m_CHECK_epicenter = ::GetPrivateProfileInt(_T("LEGEND"),str_Name,1,strIniPath);

	str_Name.Format(_T("m_CHECK_provin_border"));
	m_CHECK_provin_border = ::GetPrivateProfileInt(_T("LEGEND"),str_Name,1,strIniPath);

	str_Name.Format(_T("m_CHECK_fault"));
	m_CHECK_fault = ::GetPrivateProfileInt(_T("LEGEND"),str_Name,1,strIniPath);

	str_Name.Format(_T("m_Shape_strInfo"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("0x0000FF 50|0xFFFFFF 4|0x0000FF 4"),
		m_Shape_strInfo.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);//shpͼ����ʾ���ԣ���ɫ���߿���ע���С���ޣ�

	str_Name.Format(_T("m_StasticArea_strInfo"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("4|116.5 40 0.5 0.5|114.5 37.5 0.5 0.5|117 37 0.5 0.5|118.5 38.5 0.5 0.5"),
		m_StasticArea_strInfo.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);//ͳ������γ�ȡ���Χ��ע���С���ޣ�

	str_Name.Format(_T("m_StasticArea_SelectStyle"));
	m_StasticArea_SelectStyle = ::GetPrivateProfileInt(_T("LEGEND"),str_Name,1,strIniPath);//ͳ���������ʽ��Ĭ��Ϊ1�������Σ�

	str_Name.Format(_T("m_StasticArea_Color"));
	::GetPrivateProfileString(_T("LEGEND"),str_Name,_T("0x5effffc0"),str_Value.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);//ͳ�������ɰ�ı����ɫ������͸���ȣ�
	swscanf_s(str_Value.GetBuffer(0),_T("%x"),&m_StasticArea_Color);//CStringתunsigned long

	// OUTPUT
	str_Name.Format(_T("m_outputfolder"));
	::GetPrivateProfileString(_T("OUTPUT"),str_Name,_T(""),m_outputfolder.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	m_outputfolder.ReleaseBuffer();//ע�⣬�����ʹ��m_outputfolder.ReleaseBuffer();OnButCalculate()�е�m_outputfolder.GetLength()�᲻��ȷ 

	str_Name.Format(_T("m_selectoutputstyle"));
	m_selectoutputstyle = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,1,strIniPath);//Ĭ��Ϊ1����ִ���쳣�����Ǳ���������

	str_Name.Format(_T("m_CHEK_Clddetection2"));
	m_CHEK_Clddetection2 = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,0,strIniPath);

	str_Name.Format(_T("m_CHEK_Pseudocolor"));
	m_CHEK_Pseudocolor = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,1,strIniPath);

	str_Name.Format(_T("m_CHEK_Kpseudocolor"));
	m_CHEK_Kpseudocolor = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,0,strIniPath);

	str_Name.Format(_T("m_CHEK_Statistics"));
	m_CHEK_Statistics = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,0,strIniPath);

	str_Name.Format(_T("m_CHEK_Compound"));
	m_CHEK_Compound = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,0,strIniPath);

	str_Name.Format(_T("m_bkgndfile"));
	::GetPrivateProfileString(_T("OUTPUT"),str_Name,_T(""),m_bkgndfile.GetBuffer(MAX_PATH),MAX_PATH,strIniPath);
	
	str_Name.Format(_T("m_Channel"));
	m_Channel = ::GetPrivateProfileInt(_T("OUTPUT"),str_Name,5,strIniPath);//Ĭ��Ϊ5����Ĭ�ϴ����5ͨ��
	
}

//����������������config.ini
void CLDFProcessDlg::SetConSetting()
{
	CString str_Name,str_Value; 
	CString strIniPath = GetAppPath() + _T("Config\\Config.ini");
	/******************* INPUT
	m_selectinputstyle		ѡȡ���ݷ���ļ���or�ļ��飨0Ϊ�ļ��У�1Ϊ�ļ��飩
	m_inputfolder			ѡ����ļ���
	m_InputWindow_Extend	�Ƿ���չ��ʾ��ͼ��Ϣ
	*****************************************************/
	str_Name.Format(_T("m_selectinputstyle"));
	str_Value.Format(_T("%d"),m_selectinputstyle);
	::WritePrivateProfileString(_T("INPUT"),str_Name,str_Value,strIniPath);

	if(m_selectinputstyle==0)
	{
		str_Name.Format(_T("m_inputfolder"));
		::WritePrivateProfileString(_T("INPUT"),str_Name,m_inputfolder,strIniPath);
	}

	str_Name.Format(_T("m_InputWindow_Extend"));
	str_Value.Format(_T("%d"),m_InputWindow_Extend);
	::WritePrivateProfileString(_T("INPUT"),str_Name,str_Value,strIniPath);

	/******************* clouddetection
	m_thrld_Month[0]		upper limit�趨ͼ�����ޣ�ָ�����
	m_thrld_Month[1]~[12]	12���µ��Ƽ����ֵ
	*****************************************************/
	for(int i=0;i!=12+1;i++)
	{
		str_Name.Format(_T("m_thrld_Month[%d]"),i);
		str_Value.Format(_T("%lf"),m_thrld_Month[i]);
		::WritePrivateProfileString(_T("clouddetection"),str_Name,str_Value,strIniPath); 
	}

	/******************* abnormalmodel
	m_algorithm				ʹ�õ��㷨
	*****************************************************/
	str_Name.Format(_T("m_algorithm"));
	str_Value.Format(_T("%d"),m_algorithm);
	::WritePrivateProfileString(_T("abnormalmodel"),str_Name,str_Value,strIniPath);

	/******************* LEGEND
	m_dmin					α��ɫ��ʾͼ����Сֵ
	m_dmax					α��ɫ��ʾͼ�����ֵ
	m_dkmin					α��ɫ��ʾKֵͼ����Сֵ
	m_dkmax					α��ɫ��ʾKֵͼ�����ֵ
	m_fontsize_legend		ͼ���о�γ������Ĵ�С
	m_CHECK_epicenter		�Ƿ�������У�ʸ����
	m_CHECK_provin_border	�Ƿ����ʡ�磨ʸ����
	m_CHECK_fault			�Ƿ���Ӷϲ㣨ʸ����
	m_Shape_strInfo			shpͼ����ʾ���ԣ���ɫ���߿�
	m_StasticArea_strInfo	��Ÿ���ͳ������γ�ȡ���Χ
	m_StasticArea_SelectStyleͳ���������ʽ��Բ��or����or���ѣ�
	m_StasticArea_Color		ͳ�������ɰ�ı����ɫ������͸���ȣ�
	*****************************************************/
	str_Name.Format(_T("m_dmin"));
	str_Value.Format(_T("%lf"),m_dmin);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_dmax"));
	str_Value.Format(_T("%lf"),m_dmax);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_dkmin"));
	str_Value.Format(_T("%lf"),m_dkmin);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_dkmax"));
	str_Value.Format(_T("%lf"),m_dkmax);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_fontsize_legend"));
	str_Value.Format(_T("%d"),m_fontsize_legend);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHECK_epicenter"));
	str_Value.Format(_T("%d"),m_CHECK_epicenter);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHECK_provin_border"));
	str_Value.Format(_T("%d"),m_CHECK_provin_border);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHECK_fault"));
	str_Value.Format(_T("%d"),m_CHECK_fault);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_Shape_strInfo"));
	::WritePrivateProfileString(_T("LEGEND"),str_Name,m_Shape_strInfo,strIniPath);

	str_Name.Format(_T("m_StasticArea_strInfo"));
	::WritePrivateProfileString(_T("LEGEND"),str_Name,m_StasticArea_strInfo,strIniPath);

	str_Name.Format(_T("m_StasticArea_SelectStyle"));
	str_Value.Format(_T("%d"),m_StasticArea_SelectStyle);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_StasticArea_Color"));
	str_Value.Format(_T("%08x"),m_StasticArea_Color);
	::WritePrivateProfileString(_T("LEGEND"),str_Name,str_Value,strIniPath);

	/******************* OUTPUT
	m_outputfolder			���·��
	m_selectoutputstyle		������				������������ or �쳣��⣩
	m_CHEK_Clddetection2	�Ƿ�ʹ�������ҽ����Ƽ�⣨����������ʱ��Ч��
	m_CHEK_Pseudocolor		�Ƿ�α��ɫ��ʾ��ͼ		���쳣���ʱ��Ч��
	m_CHEK_Kpseudocolor		�Ƿ�Kֵα��ɫ��ʾ��ͼ	���쳣���ʱ��Ч��
	m_CHEK_Statistics		�Ƿ�����ͳ��			���쳣���ʱ��Ч��
	m_CHEK_Compound			�Ƿ���ںϳ�			���쳣���ʱ��Ч��
	m_bkgndfile				ѡ��ı������ļ�		����ֵ+��׼�
	m_Channel				������ͨ��
	*****************************************************/
	str_Name.Format(_T("m_outputfolder"));
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,m_outputfolder,strIniPath);

	str_Name.Format(_T("m_selectoutputstyle"));
	str_Value.Format(_T("%d"),m_selectoutputstyle);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHEK_Clddetection2"));
	str_Value.Format(_T("%d"),m_CHEK_Clddetection2);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHEK_Pseudocolor"));
	str_Value.Format(_T("%d"),m_CHEK_Pseudocolor);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHEK_Kpseudocolor"));
	str_Value.Format(_T("%d"),m_CHEK_Kpseudocolor);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHEK_Statistics"));
	str_Value.Format(_T("%d"),m_CHEK_Statistics);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_CHEK_Compound"));
	str_Value.Format(_T("%d"),m_CHEK_Compound);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	str_Name.Format(_T("m_bkgndfile"));
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,m_bkgndfile,strIniPath);

	str_Name.Format(_T("m_Channel"));
	str_Value.Format(_T("%d"),m_Channel);
	::WritePrivateProfileString(_T("OUTPUT"),str_Name,str_Value,strIniPath);

	/*����Ҫ�洢�ı���
	m_fTime=0.0;
	m_pProvider = NULL;*/
}