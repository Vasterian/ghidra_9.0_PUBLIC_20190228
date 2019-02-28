# Microsoft Developer Studio Generated NMAKE File, Based on wsspi.dsp
!IF "$(CFG)" == ""
CFG=wsspi - Win32 Debug Unicode
!MESSAGE No configuration specified. Defaulting to wsspi - Win32 Debug Unicode.
!ENDIF 

!IF "$(CFG)" != "wsspi - Win32 Debug Unicode" && "$(CFG)" != "wsspi - Win32 Release Unicode"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wsspi.mak" CFG="wsspi - Win32 Debug Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wsspi - Win32 Debug Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "wsspi - Win32 Release Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

OUTDIR=.\lib
INTDIR=.\DebugUnicode
# Begin Custom Macros
OutDir=.\lib\ 
# End Custom Macros

ALL : "$(OUTDIR)\wsspidu.lib"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\wsspidu.lib"
	-@erase ".\DebugUnicode\SspiClient.obj"
	-@erase ".\DebugUnicode\SspiEx.obj"
	-@erase ".\DebugUnicode\SspiPackage.obj"
	-@erase ".\DebugUnicode\SspiServer.obj"
	-@erase ".\DebugUnicode\StdAfx.obj"
	-@erase ".\DebugUnicode\wsspi.obj"
	-@erase ".\DebugUnicode\wsspi.pch"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"$(INTDIR)\wsspi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wsspi.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\wsspidu.lib" 
LIB32_OBJS= \
	".\DebugUnicode\SspiClient.obj" \
	".\DebugUnicode\SspiEx.obj" \
	".\DebugUnicode\SspiPackage.obj" \
	".\DebugUnicode\SspiServer.obj" \
	".\DebugUnicode\StdAfx.obj" \
	".\DebugUnicode\wsspi.obj"

"$(OUTDIR)\wsspidu.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

OUTDIR=.\lib
INTDIR=.\ReleaseUnicode
# Begin Custom Macros
OutDir=.\lib\ 
# End Custom Macros

ALL : "$(OUTDIR)\wsspiu.lib"


CLEAN :
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\wsspiu.lib"
	-@erase ".\ReleaseUnicode\SspiClient.obj"
	-@erase ".\ReleaseUnicode\SspiEx.obj"
	-@erase ".\ReleaseUnicode\SspiPackage.obj"
	-@erase ".\ReleaseUnicode\SspiServer.obj"
	-@erase ".\ReleaseUnicode\StdAfx.obj"
	-@erase ".\ReleaseUnicode\wsspi.obj"
	-@erase ".\ReleaseUnicode\wsspi.pch"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"$(INTDIR)\wsspi.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\wsspi.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\wsspiu.lib" 
LIB32_OBJS= \
	".\ReleaseUnicode\SspiClient.obj" \
	".\ReleaseUnicode\SspiEx.obj" \
	".\ReleaseUnicode\SspiPackage.obj" \
	".\ReleaseUnicode\SspiServer.obj" \
	".\ReleaseUnicode\StdAfx.obj" \
	".\ReleaseUnicode\wsspi.obj"

"$(OUTDIR)\wsspiu.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
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


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("wsspi.dep")
!INCLUDE "wsspi.dep"
!ELSE 
!MESSAGE Warning: cannot find "wsspi.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "wsspi - Win32 Debug Unicode" || "$(CFG)" == "wsspi - Win32 Release Unicode"
SOURCE=.\src\SspiClient.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\SspiClient.obj" : $(SOURCE) ".\DebugUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\SspiClient.obj" : $(SOURCE) ".\ReleaseUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\SspiEx.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\SspiEx.obj" : $(SOURCE) ".\DebugUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\SspiEx.obj" : $(SOURCE) ".\ReleaseUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\SspiPackage.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\SspiPackage.obj" : $(SOURCE) ".\DebugUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\SspiPackage.obj" : $(SOURCE) ".\ReleaseUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\SspiServer.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\SspiServer.obj" : $(SOURCE) ".\DebugUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\SspiServer.obj" : $(SOURCE) ".\ReleaseUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\StdAfx.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yc"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\StdAfx.obj"	".\DebugUnicode\wsspi.pch" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yc"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\StdAfx.obj"	".\ReleaseUnicode\wsspi.pch" : $(SOURCE)
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\wsspi.cpp

!IF  "$(CFG)" == "wsspi - Win32 Debug Unicode"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /Fp"DebugUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"DebugUnicode/" /Fd"DebugUnicode/" /FD /GZ /c 

".\DebugUnicode\wsspi.obj" : $(SOURCE) ".\DebugUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "wsspi - Win32 Release Unicode"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /Fp"ReleaseUnicode/wsspi.pch" /Yu"stdafx.h" /Fo"ReleaseUnicode/" /Fd"ReleaseUnicode/" /FD /c 

".\ReleaseUnicode\wsspi.obj" : $(SOURCE) ".\ReleaseUnicode\wsspi.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

