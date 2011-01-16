# Microsoft Developer Studio Project File - Name="bulletml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=bulletml - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "bulletml.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "bulletml.mak" CFG="bulletml - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "bulletml - Win32 Release" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE "bulletml - Win32 Debug" ("Win32 (x86) Dynamic-Link Library" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "bulletml - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BULLETML_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BULLETML_EXPORTS" /D "WIN32_DLL_EXPORT" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "bulletml - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BULLETML_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BULLETML_EXPORTS" /D "WIN32_DLL_EXPORT" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "bulletml - Win32 Release"
# Name "bulletml - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "libBulletML src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bulletml.cpp
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-tinyxml.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-xerces.cpp"
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-ygg.cpp"
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlparser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlrunner.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlrunnerimpl.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bulletmltree.cpp
# End Source File
# Begin Source File

SOURCE=.\src\calc.cpp
# End Source File
# Begin Source File

SOURCE=".\src\formula-variables.cpp"
# End Source File
# End Group
# Begin Group "TinyXML src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\tinyxml\tinyxml.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxmlerror.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tinyxml\tinyxmlparser.cpp
# End Source File
# End Group
# Begin Group "ygg src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ygg\ygg.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "libBulletML header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\auto_ptr_fix.h
# End Source File
# Begin Source File

SOURCE=.\bulletml.h
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlcommon.h
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlerror.h
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-tinyxml.h"
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-xerces.h"
# End Source File
# Begin Source File

SOURCE=".\src\bulletmlparser-ygg.h"
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlparser.h
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlrunner.h
# End Source File
# Begin Source File

SOURCE=.\src\bulletmlrunnerimpl.h
# End Source File
# Begin Source File

SOURCE=.\src\bulletmltree.h
# End Source File
# Begin Source File

SOURCE=.\src\calc.h
# End Source File
# Begin Source File

SOURCE=".\src\formula-variables.h"
# End Source File
# Begin Source File

SOURCE=.\src\formula.h
# End Source File
# Begin Source File

SOURCE=.\src\tree.h
# End Source File
# End Group
# Begin Group "TinyXML header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\tinyxml\tinyxml.h
# End Source File
# End Group
# Begin Group "ygg header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ygg\ygg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
