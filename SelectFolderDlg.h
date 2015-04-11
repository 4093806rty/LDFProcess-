/*
文件：SelectFolderDlg.h
说明：提供一个选择文件夹的对话框
*/

#ifndef SELECT_FOLDER_DLG_H
#define SELECT_FOLDER_DLG_H


#ifndef BIF_NEWDIALOGSTYLE
#define  BIF_NEWDIALOGSTYLE  0x0040
#endif


//原始方法一：
/*
class CSelectFolderDlg
{
public:
	//创建一个选择文件夹的对话框，返回所选路径
	static CString Show()
	{
		TCHAR			szFolderPath[MAX_PATH] = {0};
		CString			strFolderPath = TEXT("");
		
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot   = 0;
		sInfo.lpszTitle   = _T("请选择一个文件夹：");
		sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn     = NULL;

		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo); 
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
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




//法二：参考 http://zhidao.baidu.com/link?url=2U86qnjUL6EP5wZD0-9DgnoZNSl1hzkBQCS1bG_pLGgmrgY7odx-5r2kYfB-ha02numeDDirKGdCLlB23gD-4PdVPMb6QKpxZ5F-QOZA4ai
//实现了打开时默认目录为szPath，固定值;

/*
class CSelectFolderDlg
{

public:
	//创建一个选择文件夹的对话框，返回所选路径
	static int CALLBACK SHBrowseForFolderCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
	{
		TCHAR szPath[] = _T("C:\\Program Files");
		switch(uMsg)
		{
         
		case BFFM_INITIALIZED:    //初始化消息BFFM_INITIALIZED
			::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szPath);   //传递默认打开路径 break;
		case BFFM_SELCHANGED:    //选择路径变化，BFFM_SELCHANGED
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

		////////////////////修改使支持默认路径////////////////////////
		CString strDef(_T("D:\\aTemp\\aa\\"));  
		memcpy(szFolderPath,strDef.GetBuffer(strDef.GetLength()*2),strDef.GetLength()*2);  
		strDef.ReleaseBuffer();  
		szFolderPath[strDef.GetLength()]=0;  
		//////////////////////////////////////////////////////////////


		CString			strFolderPath = TEXT("");
		
		BROWSEINFO		sInfo;
		::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
		sInfo.pidlRoot   = 0;
		sInfo.lpszTitle   = _T("请选择一个文件夹：");
		sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
		sInfo.lpfn = SHBrowseForFolderCallbackProc ;        //设置CALLBACK函数//NULL;


		////////////////////修改使支持默认路径////////////////////////
		sInfo.iImage = 0;  
		sInfo.lParam = long(&szFolderPath);    //设置默认路径 
		//////////////////////////////////////////////////////////////


		// 显示文件夹选择对话框
		LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo); 
		if (lpidlBrowse != NULL)
		{
			// 取得文件夹名
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

//法三：参考 http://blog.csdn.net/shuilan0066/article/details/7302904
//实现了记住上一次打开的目录m_filePath，并且首次打开时默认目录为m_filePath;
//效果最好，但是由于不会声明静态变量，（可能是H文件被重复包含）增加了一个SelectFolderDlg.cpp文件
//并且在stdafx.h中加入了一句 #include "SelectFolderDlg.h"，有时间再改吧
//可以参考 http://www.cnblogs.com/greatverve/archive/2012/12/16/SHBrowseForFolder-CFileDialog.html
class CSelectFolderDlg
{
	public:  
	static CString m_filePath;

    static  int CALLBACK  BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)  
    {  
        switch(uMsg)  
        {  
        case BFFM_INITIALIZED:    //初始化消息  
            ::SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)(LPTSTR)(LPCTSTR)m_filePath);   //  m_filePath 为类的静态变量  
            break;  
        case BFFM_SELCHANGED:    //选择路径变化，  
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
		bi.lpszTitle = L"选择路径";  
		//BIF_NEWDIALOGSTYLE会有“新建文件夹”按钮，但是这种对话框的viewport不会的定位到所选择的默认文件夹上，更换同目录的其他文件夹时需拖动滚动条
		//BIF_STATUSTEXT 会显示选择文件夹的全路径
		//BIF_RETURNONLYFSDIRS 回收站、控制面板就不显示了
		bi.ulFlags = BIF_STATUSTEXT |  BIF_RETURNONLYFSDIRS ;// | BIF_EDITBOX
		bi.lpfn = BrowseCallbackProc ;        //设置CALLBACK函数  
		bi.iImage = 0;  
		//bi.lParam = long(&szDefaultDir);    //设置默认路径  
      
		LPMALLOC pMalloc;  
		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);  
      
		if(pidl != NULL)  
		{  
			TCHAR * path = new TCHAR[MAX_PATH];   
          
			SHGetPathFromIDList(pidl,path);  
			if(SUCCEEDED(SHGetMalloc(&pMalloc)))//pidl指向的对象用完应该释放  
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


