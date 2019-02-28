// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__AE42ED70_D682_40DA_B048_8712D7704660__INCLUDED_)
#define AFX_STDAFX_H__AE42ED70_D682_40DA_B048_8712D7704660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined(_UNICODE)
#define UNICODE
#endif

#include <assert.h>
#include <vector>


#ifndef _WINDOWS_
#include <windows.h>
#endif 

#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifndef _EXCEPTION_
#include <exception>
#endif 

#ifndef _STRING_
#include <string>
#endif 

#ifndef _OSTREAM_
#include <ostream>
#endif 

#ifndef __SSPI_H__
#define SECURITY_WIN32
#include <sspi.h>
#endif

#include <security.h>

namespace wsspi
{
#ifdef _UNICODE
    typedef std::wstring  wsspi_string;  
    typedef std::wostream wsspi_ostream;
#else
    typedef std::string  wsspi_string;
    typedef std::ostream wsspi_ostream;
#endif
}
// private headers
#include "..\inc\SspiEx.h"

// internal global data and functions
namespace wsspi
{
    void SspiInitialize ( );
    void SspiUninitialize ( );
    extern PSecurityFunctionTable  _pSecurityInterface;      // security interface table
    // inline functions for initializing lib
    inline void InitLib ( )
    {
        if ( _pSecurityInterface == NULL )
            SspiInitialize ( );
    }
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__AE42ED70_D682_40DA_B048_8712D7704660__INCLUDED_)
