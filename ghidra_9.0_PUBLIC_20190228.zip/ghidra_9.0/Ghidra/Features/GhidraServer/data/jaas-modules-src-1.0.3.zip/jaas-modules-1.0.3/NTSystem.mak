# Microsoft Developer Studio Generated NMAKE File, Based on NTSystem.dsp
!IF "$(CFG)" == ""
CFG=NTSystem - Win32 Release Unicode
!MESSAGE No configuration specified. Defaulting to NTSystem - Win32 Release Unicode.
!ENDIF 

!IF "$(CFG)" != "NTSystem - Win32 Release Unicode" && "$(CFG)" != "NTSystem - Win32 Debug Unicode"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NTSystem.mak" CFG="NTSystem - Win32 Release Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NTSystem - Win32 Release Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NTSystem - Win32 Debug Unicode" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NTSystem - Win32 Release Unicode"

OUTDIR=.\ReleaseUnicode
INTDIR=.\ReleaseUnicode
# Begin Custom Macros
OutDir=.\ReleaseUnicode
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\NTSystem.dll" "$(OUTDIR)\NTSystem.bsc"

!ELSE 

ALL : "wsspi - Win32 Release Unicode" ".\NTSystem.dll" "$(OUTDIR)\NTSystem.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"wsspi - Win32 Release UnicodeCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\NTSystem.obj"
	-@erase "$(INTDIR)\NTSystem.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\NTSystem.bsc"
	-@erase "$(OUTDIR)\NTSystem.exp"
	-@erase "$(OUTDIR)\NTSystem.lib"
	-@erase ".\NTSystem.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "C:\JBuilder4\jdk1.3\include" /I "C:\JBuilder4\jdk1.3\include\win32" /I "wsspi\inc" /I "C:\jbuilder5\jdk1.3\include" /I "C:\jbuilder5\jdk1.3\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NTSYSTEM_EXPORTS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NTSystem.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\NTSystem.sbr"

"$(OUTDIR)\NTSystem.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsspiu.lib /nologo /dll /incremental:no /pdb:"$(OUTDIR)\NTSystem.pdb" /machine:I386 /out:"NTSystem.dll" /implib:"$(OUTDIR)\NTSystem.lib" /libpath:"wsspi\lib" 
LINK32_OBJS= \
	"$(INTDIR)\NTSystem.obj" \
	".\Wsspi\lib\wsspiu.lib"

".\NTSystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "NTSystem - Win32 Debug Unicode"

OUTDIR=.\DebugUnicode
INTDIR=.\DebugUnicode
# Begin Custom Macros
OutDir=.\DebugUnicode
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : ".\NTSystem.dll" "$(OUTDIR)\NTSystem.bsc"

!ELSE 

ALL : "wsspi - Win32 Debug Unicode" ".\NTSystem.dll" "$(OUTDIR)\NTSystem.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"wsspi - Win32 Debug UnicodeCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\NTSystem.obj"
	-@erase "$(INTDIR)\NTSystem.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\NTSystem.bsc"
	-@erase "$(OUTDIR)\NTSystem.exp"
	-@erase "$(OUTDIR)\NTSystem.lib"
	-@erase "$(OUTDIR)\NTSystem.pdb"
	-@erase ".\NTSystem.dll"
	-@erase ".\NTSystem.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /WX /GX /Zi /Od /I "C:\JBuilder4\jdk1.3\include" /I "C:\JBuilder4\jdk1.3\include\win32" /I "wsspi\inc" /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" /I "C:\jbuilder5\jdk1.3\include" /I "C:\jbuilder5\jdk1.3\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NTSYSTEM_EXPORTS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\NTSystem.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\NTSystem.sbr"

"$(OUTDIR)\NTSystem.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsspidu.lib /nologo /dll /incremental:yes /pdb:"$(OUTDIR)\NTSystem.pdb" /debug /machine:I386 /out:"NTSystem.dll" /implib:"$(OUTDIR)\NTSystem.lib" /libpath:"wsspi\lib" 
LINK32_OBJS= \
	"$(INTDIR)\NTSystem.obj" \
	".\Wsspi\lib\wsspidu.lib"

".\NTSystem.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 

!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("NTSystem.dep")
!INCLUDE "NTSystem.dep"
!ELSE 
!MESSAGE Warning: cannot find "NTSystem.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "NTSystem - Win32 Release Unicode" || "$(CFG)" == "NTSystem - Win32 Debug Unicode"
SOURCE=.\NTSystem.cpp

!IF  "$(CFG)" == "NTSystem - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /I "wsspi\inc" /I "C:\jbuilder5\jdk1.3\include" /I "C:\jbuilder5\jdk1.3\include\win32" /I "C:\JBuilder7\jdk1.3.1\include" /I "C:\JBuilder7\jdk1.3.1\include\win32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NTSYSTEM_EXPORTS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NTSystem.obj"	"$(INTDIR)\NTSystem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "NTSystem - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /WX /GX /Zi /Od /I "wsspi\inc" /I "%JAVA_HOME%\include" /I "%JAVA_HOME%\include\win32" /I "C:\jbuilder5\jdk1.3\include" /I "C:\jbuilder5\jdk1.3\include\win32" /I "C:\JBuilder7\jdk1.3.1\include" /I "C:\JBuilder7\jdk1.3.1\include\win32" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NTSYSTEM_EXPORTS" /FR"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\NTSystem.obj"	"$(INTDIR)\NTSystem.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

!IF  "$(CFG)" == "NTSystem - Win32 Release Unicode"

"wsspi - Win32 Release Unicode" : 
   cd ".\Wsspi"
   $(MAKE) /$(MAKEFLAGS) /F .\wsspi.mak CFG="wsspi - Win32 Release Unicode" 
   cd ".."

"wsspi - Win32 Release UnicodeCLEAN" : 
   cd ".\Wsspi"
   $(MAKE) /$(MAKEFLAGS) /F .\wsspi.mak CFG="wsspi - Win32 Release Unicode" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "NTSystem - Win32 Debug Unicode"

"wsspi - Win32 Debug Unicode" : 
   cd ".\Wsspi"
   $(MAKE) /$(MAKEFLAGS) /F .\wsspi.mak CFG="wsspi - Win32 Debug Unicode" 
   cd ".."

"wsspi - Win32 Debug UnicodeCLEAN" : 
   cd ".\Wsspi"
   $(MAKE) /$(MAKEFLAGS) /F .\wsspi.mak CFG="wsspi - Win32 Debug Unicode" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 


!ENDIF 

