/*
�ļ���SelectFolderDlg.h
˵�����ṩһ��ѡ���ļ��еĶԻ���
*/

#ifndef SELECT_FOLDER_DLG_H
#define SELECT_FOLDER_DLG_H


#ifndef BIF_NEWDIALOGSTYLE
#define  BIF_NEWDIALOGSTYLE  0x0040
#endif


//ԭʼ����һ��
/*
class CSelectFolderDlg
{
public:
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��
	static CString Show()
	{
		TCHAR			szFolderPath[MAX_PATH] = {0};
		CString			strFolderPath = TEXT("");
		
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot   = 0;
		sInfo.lpszTitle   = _T("��ѡ��һ���ļ��У�");
		sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn     = NULL;

		// ��ʾ�ļ���ѡ��Ի���
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo); 
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����
			if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))  
			{
				strFolderPath = szFolderPath;
			}
		}
		if(lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}

};
*/




//�������ο� http://zhidao.baidu.com/link?url=2U86qnjUL6EP5wZD0-9DgnoZNSl1hzkBQCS1bG_pLGgmrgY7odx-5r2kYfB-ha02numeDDirKGdCLlB23gD-4PdVPMb6QKpxZ5F-QOZA4ai
//ʵ���˴�ʱĬ��Ŀ¼ΪszPath���̶�ֵ;

/*
class CSelectFolderDlg
{

public:
	//����һ��ѡ���ļ��еĶԻ��򣬷�����ѡ·��
	static int CALLBACK SHBrowseForFolderCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
	{
		TCHAR szPath[] = _T("C:\\Program Files");
		switch(uMsg)
		{
         
		case BFFM_INITIALIZED:    //��ʼ����ϢBFFM_INITIALIZED
			::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //����Ĭ�ϴ�·�� break;
		case BFFM_SELCHANGED:    //ѡ��·���仯��BFFM_SELCHANGED
			{
				TCHAR curr[MAX_PATH];   
				SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);   
				::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)&curr[0]);  
			}
			break;
		default:
			break;
		} 
		return 0;
	}



	static CString Show()
	{
		
		TCHAR	szFolderPath[MAX_PATH] = {0};

		////////////////////�޸�ʹ֧��Ĭ��·��////////////////////////
		CString strDef(_T("D:\\aTemp\\aa\\"));  
		memcpy(szFolderPath,strDef.GetBuffer(strDef.GetLength()*2),strDef.GetLength()*2);  
		strDef.ReleaseBuffer();  
		szFolderPath[strDef.GetLength()]=0;  
		//////////////////////////////////////////////////////////////


		CString			strFolderPath = TEXT("");
		
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot   = 0;
		sInfo.lpszTitle   = _T("��ѡ��һ���ļ��У�");
		sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = SHBrowseForFolderCallbackProc ;        //����CALLBACK����//NULL;


		////////////////////�޸�ʹ֧��Ĭ��·��////////////////////////
		sInfo.iImage = 0;  
		sInfo.lParam = long(&szFolderPath);    //����Ĭ��·�� 
		//////////////////////////////////////////////////////////////


		// ��ʾ�ļ���ѡ��Ի���
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo); 
		if (lpidlBrowse != NULL)
		{
			// ȡ���ļ�����
			if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))  
			{
				strFolderPath = szFolderPath;
			}
		}
		if(lpidlBrowse != NULL)
		{
			::CoTaskMemFree(lpidlBrowse);
		}

		return strFolderPath;

	}
	
};
*/

//�������ο� http://blog.csdn.net/shuilan0066/article/details/7302904
//ʵ���˼�ס��һ�δ򿪵�Ŀ¼m_filePath�������״δ�ʱĬ��Ŀ¼Ϊm_filePath;
//Ч����ã��������ڲ���������̬��������������H�ļ����ظ�������������һ��SelectFolderDlg.cpp�ļ�
//������stdafx.h�м�����һ�� #include "SelectFolderDlg.h"����ʱ���ٸİ�
//���Բο� http://www.cnblogs.com/greatverve/archive/2012/12/16/SHBrowseForFolder-CFileDialog.html
class CSelectFolderDlg
{
	public:  
	static CString m_filePath;

    static  int CALLBACK  BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
    {  
        switch(uMsg)  
        {  
        case BFFM_INITIALIZED:    //��ʼ����Ϣ  
            ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)(LPTSTR)(LPCTSTR)m_filePath);   //  m_filePath Ϊ��ľ�̬����  
            break;  
        case BFFM_SELCHANGED:    //ѡ��·���仯��  
            {  
                TCHAR curr[MAX_PATH];     
                SHGetPathFromIDList((LPCITEMIDLIST)lParam,curr);     
                ::SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)curr);     
            }  
            break;  
        default:  
            break;  
        }  
        return 0;     
    }


	static CString Show()
	{
		BROWSEINFO bi;  
		ZeroMemory(&bi,sizeof(BROWSEINFO));  
      
		/*TCHAR szDefaultDir[MAX_PATH];  
		CString strDef(_T("C:\\WINDOWS\\"));  
		memcpy(szDefaultDir,strDef.GetBuffer(strDef.GetLength()*2),strDef.GetLength()*2);  
		strDef.ReleaseBuffer();  
		szDefaultDir[strDef.GetLength()]=0;  */
      

		bi.pidlRoot = 0;  
		bi.pszDisplayName = 0;  
		bi.lpszTitle = L"ѡ��·��";  
		//BIF_NEWDIALOGSTYLE���С��½��ļ��С���ť���������ֶԻ����viewport����Ķ�λ����ѡ���Ĭ���ļ����ϣ�����ͬĿ¼�������ļ���ʱ���϶�������
		//BIF_STATUSTEXT ����ʾѡ���ļ��е�ȫ·��
		//BIF_RETURNONLYFSDIRS ����վ���������Ͳ���ʾ��
		bi.ulFlags = BIF_STATUSTEXT |  BIF_RETURNONLYFSDIRS ;// | BIF_EDITBOX
		bi.lpfn = BrowseCallbackProc ;        //����CALLBACK����  
		bi.iImage = 0;  
		//bi.lParam = long(&szDefaultDir);    //����Ĭ��·��  
      
		LPMALLOC pMalloc;  
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
      
		if(pidl != NULL)  
		{  
			TCHAR * path = new TCHAR[MAX_PATH];   
          
			SHGetPathFromIDList(pidl,path);  
			if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pidlָ��Ķ�������Ӧ���ͷ�  
			{  
				pMalloc->Free(pidl);  
				pMalloc->Release();  
			}  
			m_filePath=path;  
      
			delete [] path;  
      
		}  
		return m_filePath;
	}
	
	
};


#endif


