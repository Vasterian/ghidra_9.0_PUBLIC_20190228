//==============================================================================
// File:		wsspi.cpp
//
// Description:	global functions and variables of the library
//
// Revisions: 	Created: 11/24/1999
//
//==============================================================================
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//==============================================================================
#include "stdafx.h"

using namespace wsspi;

// global data
namespace wsspi
{
    PSecurityFunctionTable  _pSecurityInterface = NULL;      // security interface table
}

// the provider dll's instance is not exported outside of this module
namespace
{
    HINSTANCE               _hProvider          = NULL;      // provider dll's instance
} 
#ifdef _UNICODE
#define INIT_SEC_INTERFACE_NAME       "InitSecurityInterfaceW"
#else
#define INIT_SEC_INTERFACE_NAME       "InitSecurityInterfaceA"
#endif

namespace wsspi
{
    void SspiInitialize ( )
    {
        // load the provider dll
        _hProvider = LoadLibrary ( _T("security.dll") );
        if ( _hProvider == NULL )
            THROWEX ( ErrorNoLibrary );
    

        INIT_SECURITY_INTERFACE InitSecurityInterface;

        // Get the address of the InitSecurityInterface function.
        InitSecurityInterface = reinterpret_cast<INIT_SECURITY_INTERFACE> (
                                    GetProcAddress (
                                            _hProvider, 
                                            INIT_SEC_INTERFACE_NAME
                                         ) );
        if ( InitSecurityInterface == NULL )
            THROWEX ( ErrorNoSecurityInterface );

        _pSecurityInterface = InitSecurityInterface ( );
        if ( _pSecurityInterface == NULL )
            THROWEX ( ErrorNoSecurityInterface );
 
    }

    void SspiUninitialize ( )
    {
        FreeLibrary ( _hProvider );
        _hProvider = NULL;
        _pSecurityInterface = NULL;
    }
} // namespace wsppi