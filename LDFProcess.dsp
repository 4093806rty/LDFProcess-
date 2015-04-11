# Microsoft Developer Studio Project File - Name="LDFProcess" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=LDFProcess - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LDFProcess.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LDFProcess.mak" CFG="LDFProcess - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LDFProcess - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "LDFProcess - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LDFProcess - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "LDFProcess - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LDFProcess - Win32 Release"
# Name "LDFProcess - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AbnmdlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CldetcDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\Core.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\GDC.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoMath.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\GMap.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\IProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\Irmath.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\LDF.cpp
# End Source File
# Begin Source File

SOURCE=.\LDFProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\LDFProcess.rc
# End Source File
# Begin Source File

SOURCE=.\LDFProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LDFToPicture.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\ShapeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Lib\ShapeProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Lib\Store.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Lib\_ShapeFile.h
# End Source File
# Begin Source File

SOURCE=.\AbnmdlDlg.h
# End Source File
# Begin Source File

SOURCE=.\CldetcDlg.h
# End Source File
# Begin Source File

SOURCE=.\Lib\Core.h
# End Source File
# Begin Source File

SOURCE=.\Lib\GDC.h
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoLayer.h
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoMath.h
# End Source File
# Begin Source File

SOURCE=.\Lib\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\Lib\GeoTable.h
# End Source File
# Begin Source File

SOURCE=.\Lib\GMap.h
# End Source File
# Begin Source File

SOURCE=.\InputDlg.h
# End Source File
# Begin Source File

SOURCE=.\Lib\IProvider.h
# End Source File
# Begin Source File

SOURCE=.\Lib\Irmath.h
# End Source File
# Begin Source File

SOURCE=.\Lib\LDF.h
# End Source File
# Begin Source File

SOURCE=.\LDFProcess.h
# End Source File
# Begin Source File

SOURCE=.\LDFProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\LDFToPicture.h
# End Source File
# Begin Source File

SOURCE=.\OutputDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectFolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\Lib\ShapeFile.h
# End Source File
# Begin Source File

SOURCE=.\Lib\ShapeProvider.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Lib\Store.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\LDFProcess.ico
# End Source File
# Begin Source File

SOURCE=.\res\LDFProcess.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
