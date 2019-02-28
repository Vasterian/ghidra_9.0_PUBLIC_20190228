//==============================================================================
// File:		wsspi.h
//
// Description:	main include file for the wsspi class library
//
// Revisions: 	Created: 11/20/1999
//
//==============================================================================
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//==============================================================================

#if !defined(_WSSPI_H_)
#define _WSSPI_H_

#if defined(_UNICODE)
#define UNICODE
#endif

#if !defined(WSSPI_NO_WINHEADERS)

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

#ifndef _VECTOR_
#include <vector>
#endif

#endif // WSSPI_NO_WINHEADERS

#if !defined(WSSPI_NO_AUTO_INCLUDE)

#ifdef UNICODE
#ifdef _DEBUG
#pragma comment(lib, "wsspidu.lib")
#else
#pragma comment(lib, "wsspiu.lib")
#endif // _DEBUG
#else  // UNICODE
#ifdef _DEBUG
#pragma comment(lib, "wsspid.lib")
#else
#pragma comment(lib, "wsspi.lib")
#endif // _DEBUG
#endif // UNICODE

// global functions and typedefs:
namespace wsspi
{
#ifdef _UNICODE
    typedef std::wstring  wsspi_string;  
    typedef std::wostream wsspi_ostream;
#else
    typedef std::string  wsspi_string;
    typedef std::ostream wsspi_ostream;
#endif

    void SspiInitialize ( );
    void SspiUninitialize ( );
}
// include all classes
#include "SspiPackage.h"
#include "SspiDef.h"
#include "SspiEx.h"
#include "SspiServer.h"
#include "SspiClient.h"

#endif // WSSPI_NO_AUTO_INCLUDE

#endif // _WSSPI_H_