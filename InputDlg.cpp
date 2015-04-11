// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LDFProcess.h"
#include "InputDlg.h"
#include "SelectFolderDlg.h"
#include ".\lib\LDF.h"
#include "LDFProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//���޸�������Input����Ӱ�ť������ӵ��ļ���Ŀ����
#define MAX_CFileDialog_FILE_COUNT 999//�����п�֪����������Ϊ�����ļ�ȫ·�����ƴﵽ�ʱ����֧��ѡȡ999���ļ���ʵ���϶��ڴˣ�
#define FILE_LIST_BUFFER_SIZE ((MAX_CFileDialog_FILE_COUNT * (MAX_PATH + 1)) + 1)

/////////////////////////////////////////////////////////////////////////////
// CInputDlg dialog


CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDlg)
	m_inputfolder = _T("");
	m_minLon = _T("");
	m_maxLon = _T("");
	m_resLon = _T("");
	m_minLat = _T("");
	m_maxLat = _T("");
	m_resLat = _T("");
	m_selectinputstyle = 0;//0Ϊ�ļ��У�1Ϊ�ļ���
	//}}AFX_DATA_INIT
	rectSmall.left=0;
	rectSmall.top=0;
	rectSmall.right=0;
	rectSmall.bottom=0;
	rectLarge.left=0;
	rectLarge.top=0;
	rectLarge.right=0;
	rectLarge.bottom=0;
	m_InputWindow_Extend=FALSE;//Ĭ��Ϊ0������չ���봰�ڣ�������ʾ��ͼ��Ϣ��
}


void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDlg)
	DDX_Control(pDX, IDC_LIST_FILE, m_listFile);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_inputfolder);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_minLon);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_maxLon);
	DDX_Text(pDX, IDC_EDIT_LONRES, m_resLon);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_minLat);
	DDX_Text(pDX, IDC_EDIT_TOP, m_maxLat);
	DDX_Text(pDX, IDC_EDIT_LATRES, m_resLat);
	DDX_Radio(pDX, IDC_RADIO_Folder, m_selectinputstyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	//{{AFX_MSG_MAP(CInputDlg)
	ON_BN_CLICKED(IDC_BUT_SEL, OnButSel)
	ON_BN_CLICKED(IDC_RADIO_Folder, OnRADIOFolder)
	ON_BN_CLICKED(IDC_RADIO_Files, OnRADIOFiles)
	ON_BN_CLICKED(IDC_BUT_Add, OnBUTAdd)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, OnSelchangeListFile)
	ON_BN_CLICKED(IDC_BUT_Del, OnBUTDel)
	ON_BN_CLICKED(IDC_BUT_Clear, OnBUTClear)
	ON_BN_CLICKED(IDC_BUT_Extend, OnBUTExtend)
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, &CInputDlg::OnEnKillfocusEditInput)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDlg message handlers

void CInputDlg::OnButSel() 
{
	// TODO: Add your control notification handler code here
	//�״δ�ʱ��Ҫ����һ��Ĭ��Ŀ¼��֮����Զ���ס�ϴ�ѡ����ļ���
	if (CSelectFolderDlg::m_filePath.GetLength()==0) CSelectFolderDlg::m_filePath=m_inputfolder;
	m_inputfolder=CSelectFolderDlg::Show();
    CFileFind ff;
    BOOL ret = ff.FindFile(m_inputfolder+"\\*.LDF");
    while(ret)
    {
        ret = ff.FindNextFile();
		if(!m_repeatcheck.Lookup(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName())){
			m_repeatcheck.SetAt(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName());
			m_listFile.AddString(m_inputfolder+"\\"+ff.GetFileName());
		}
    }
    ff.Close();

	//��ʾѡ����ļ���Ŀ
	ShowFilesNum();

	UpdateData(FALSE);
}

//ѡȡ���ݷ���ļ���
void CInputDlg::OnRADIOFolder() 
{
	// TODO: Add your control notification handler code here
	m_selectinputstyle=0;
	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow();
	GetDlgItem(IDC_BUT_SEL)->EnableWindow();

	GetDlgItem(IDC_BUT_Add)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_Del)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_Clear)->EnableWindow(FALSE);
}

//ѡȡ���ݷ���ļ���
void CInputDlg::OnRADIOFiles() 
{
	// TODO: Add your control notification handler code here
	m_selectinputstyle=1;
	GetDlgItem(IDC_BUT_Add)->EnableWindow();
	GetDlgItem(IDC_BUT_Del)->EnableWindow();
	GetDlgItem(IDC_BUT_Clear)->EnableWindow();

	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUT_SEL)->EnableWindow(FALSE);
}

void CInputDlg::OnBUTAdd() 
{
	
	if(m_strLastDir_InputFiles.GetLength()==0)
		{m_strLastDir_InputFiles = _T("D:\\aTemp\\aa"); }//��ǰ"D:/Temp/aa"����д��Ҳ�ǿ��Ե�

	// TODO: Add your control notification handler code here
	DWORD flag = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT;
	CString str_Selectfile;

	CString fileName;
	wchar_t * p = fileName.GetBuffer(FILE_LIST_BUFFER_SIZE);

	CFileDialog dlgFile(TRUE,  _T("LDF"), _T("*.LDF"), flag,
		TEXT("֧�ֵ��ļ� (*.ldf)|*.ldf|�²�֧�ֵ��ļ�(*.gpf,*.tiff;*.tif)|*.gpf,*.tiff;*.tif|�����ļ�(*.*)|*.*||"),NULL);//_T("LDF(*.LDF)|*.ldf||"));

	OPENFILENAME& ofn = dlgFile.m_ofn;

	ofn.lStructSize = 88;//76����win98���
	ofn.lpstrTitle=TEXT("���LDF�ļ�...\0"); 
	ofn.lpstrFile = p;
	ofn.nMaxFile = FILE_LIST_BUFFER_SIZE;
	ofn.lpstrInitialDir=m_strLastDir_InputFiles;//��һ�δ򿪵��ļ���
	/**************************************
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	int nResult = ::GetOpenFileName(&ofn);
	/*************************************/
	if (dlgFile.DoModal() == IDOK){
		POSITION pos = dlgFile.GetStartPosition();
		while (pos){
			str_Selectfile = dlgFile.GetNextPathName(pos);
			if(!m_repeatcheck.Lookup(str_Selectfile,str_Selectfile)){
				m_repeatcheck.SetAt(str_Selectfile,str_Selectfile);
				m_listFile.AddString(str_Selectfile);
			}
		}
		//�洢�ϴδ��ļ���
		m_strLastDir_InputFiles = str_Selectfile.Left(str_Selectfile.ReverseFind('\\'));
		UpdateData(FALSE);
	}

	//��ʾѡ����ļ���Ŀ
	ShowFilesNum();

	//fileName.ReleaseBuffer();

	if (m_listFile.GetCount() >= 1){
		//Ĭ����ʾ��һ���ļ�����Ϣ��ͼ��
		LDF ldf1;
		CString str;
		m_listFile.GetText(0, str);

		//��ͷ�ļ�ȷ����ͼ��Ϣ��6������
		if (!ldf1.readHdr(str)){
			return;
		}
		m_resLon.Format(_T("%.4f"),ldf1.m_hdr.fLonSolution);
		m_resLat.Format(_T("%.4f"),ldf1.m_hdr.fLatSolution);
		m_minLon.Format(_T("%.4f"),ldf1.m_hdr.fMinLon);
		m_maxLon.Format(_T("%.4f"),ldf1.m_hdr.fMaxLon);
		m_minLat.Format(_T("%.4f"),ldf1.m_hdr.fMinLat);
		m_maxLat.Format(_T("%.4f"),ldf1.m_hdr.fMaxLat);
		UpdateData(FALSE);
	}
}

void CInputDlg::OnSelchangeListFile() 
{
	// TODO: Add your control notification handler code here
	LDF ldf1;
	CString str;
	int n = m_listFile.GetCurSel();
	if (n <= m_listFile.GetCount()-1 && n!=-1)
	{
		m_listFile.GetText(n, str);
		if (!ldf1.readHdr(str)){
			return;
		}
		m_resLon.Format(_T("%.4f"),ldf1.m_hdr.fLonSolution);
		m_resLat.Format(_T("%.4f"),ldf1.m_hdr.fLatSolution);
		m_minLon.Format(_T("%.4f"),ldf1.m_hdr.fMinLon);
		m_maxLon.Format(_T("%.4f"),ldf1.m_hdr.fMaxLon);
		m_minLat.Format(_T("%.4f"),ldf1.m_hdr.fMinLat);
		m_maxLat.Format(_T("%.4f"),ldf1.m_hdr.fMaxLat);
	}
	UpdateData(FALSE);
}

void CInputDlg::OnBUTDel() 
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSel = m_listFile.GetCurSel();
	if (nSel == -1){
		return;
	}
	m_listFile.GetText(nSel,str);
	m_repeatcheck.RemoveKey(str); 
	m_listFile.DeleteString(nSel);
	if (nSel > 0)
	{
		m_listFile.SetCurSel(nSel - 1);
	}
	else{
		m_listFile.SetCurSel(0);
	}
	//���¼���ѡ����ļ���Ŀ
	ShowFilesNum();
}

void CInputDlg::OnBUTClear() 
{
	// TODO: Add your control notification handler code here
	while (m_listFile.GetCount() > 0){
		m_listFile.DeleteString(m_listFile.GetCount() - 1);
		//���¼���ѡ����ļ���Ŀ
		ShowFilesNum();
	}
	m_repeatcheck.RemoveAll();
}

BOOL CInputDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_selectinputstyle=((CLDFProcessDlg*)GetParent())->m_selectinputstyle;
	if(0==m_selectinputstyle){
		//((CButton*)GetDlgItem(IDC_RADIO_Folder))->SetCheck(TRUE);
		GetDlgItem(IDC_BUT_Add)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_Del)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_Clear)->EnableWindow(FALSE);
	}
	else{
		//((CButton*)GetDlgItem(IDC_RADIO_Files))->SetCheck(TRUE);
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUT_SEL)->EnableWindow(FALSE);}

	for(int i=0;i<((CLDFProcessDlg*)GetParent())->m_strArray.GetSize();i++){
		m_listFile.AddString(((CLDFProcessDlg*)GetParent())->m_strArray[i]);
		m_repeatcheck.SetAt(((CLDFProcessDlg*)GetParent())->m_strArray[i],((CLDFProcessDlg*)GetParent())->m_strArray[i]);
	}
	m_inputfolder=((CLDFProcessDlg*)GetParent())->m_inputfolder;
	m_InputWindow_Extend = ((CLDFProcessDlg*)GetParent())->m_InputWindow_Extend;
	
	//���ȱ���ȫ����ʱ�Ĵ��ڴ�С��Ϣ
	CRect rectSeparator;
	GetWindowRect(&rectLarge);
	GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

	rectSmall.left=rectLarge.left;
	rectSmall.top=rectLarge.top;
	rectSmall.right=rectSeparator.right;
	rectSmall.bottom=rectLarge.bottom;	

	//��ȫ������ʾ����С������ʾ
	if(!m_InputWindow_Extend)//rectLarge.IsRectNull()
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
		SetDlgItemText(IDC_BUT_Extend,_T("Info>>"));
	}
	else
	{
		SetDlgItemText(IDC_BUT_Extend,_T("����<<"));
	}

	//��ʾѡ����ļ���Ŀ
	ShowFilesNum();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInputDlg::OnBUTExtend() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if(GetDlgItemText(IDC_BUT_Extend,str),str=="����<<")
	{
		//�޸�
		SetDlgItemText(IDC_BUT_Extend,_T("Info>>"));
	}
	else
	{
		//�޸�
		SetDlgItemText(IDC_BUT_Extend,_T("����<<"));
	}

	if(str=="����<<")
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	if(m_InputWindow_Extend)
		m_InputWindow_Extend=FALSE;
	else
		m_InputWindow_Extend=TRUE;
}

void CInputDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	((CLDFProcessDlg*)GetParent())->m_selectinputstyle=m_selectinputstyle;
	((CLDFProcessDlg*)GetParent())->m_inputfolder=m_inputfolder;

	((CLDFProcessDlg*)GetParent())->m_strArray.RemoveAll();
	CString strtemp;
	for(int i=0;i<m_listFile.GetCount();i++){
		m_listFile.GetText(i,strtemp);
		((CLDFProcessDlg*)GetParent())->m_strArray.Add(strtemp);
	}

	((CLDFProcessDlg*)GetParent())->m_InputWindow_Extend = m_InputWindow_Extend;

	CDialog::OnOK();
}

void CInputDlg::OnEnKillfocusEditInput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
    CFileFind ff;
    BOOL ret = ff.FindFile(m_inputfolder+"\\*.LDF");
    while(ret)
    {
        ret = ff.FindNextFile();
		if(!m_repeatcheck.Lookup(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName())){
			m_repeatcheck.SetAt(m_inputfolder+"\\"+ff.GetFileName(),m_inputfolder+"\\"+ff.GetFileName());
			m_listFile.AddString(m_inputfolder+"\\"+ff.GetFileName());
		}
    }
    ff.Close();

	//��ʾѡ����ļ���Ŀ
	ShowFilesNum();

	UpdateData(FALSE);
}


//��ʾѡ����ļ���Ŀ
void CInputDlg::ShowFilesNum() 
{
	//����ѡ����ļ���Ŀ
	int n = m_listFile.GetCount();
	CString buf;
	if (n !=0)
		buf.Format(_T("�� %d ���ļ�"), n);
	else
		buf=_T("");
	GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(buf);
}

