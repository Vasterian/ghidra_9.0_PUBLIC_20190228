//==============================================================================
// File:		sspi_test.cpp
// Project:		sspi_test
//
// Description:	test for SSPI authentication
//
// Revisions: 	Created: 11/17/1999
//
//==============================================================================
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//==============================================================================

#include "stdafx.h"
#include <iostream>
#include "wsspi.h"

using namespace wsspi;

BOOL EnableTokenPrivilege ( LPTSTR privilege )
{
	HANDLE		hToken;				// process token 
	TOKEN_PRIVILEGES tp;			// token provileges
	DWORD		dwSize;				

	// initialize privilege structure
	ZeroMemory (&tp, sizeof (tp));
	tp.PrivilegeCount = 1;

	// open the process token
	if ( !OpenProcessToken ( GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken ) )
		return FALSE;
	
	// look up the privilege LUID and enable it
	if ( !LookupPrivilegeValue ( NULL, privilege, &tp.Privileges[0].Luid ) )
	{ 
		CloseHandle ( hToken);
		return FALSE;
	}
	
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// adjust token privileges
	AdjustTokenPrivileges ( hToken, FALSE, &tp, 0, NULL, &dwSize );

    DWORD err = GetLastError ( );
    if ( err != ERROR_SUCCESS )
	{ 
        printf ( "PRIVILEGE: %lu\n", err );
        CloseHandle ( hToken );
		return FALSE;
	}

	// clean up
	CloseHandle ( hToken );
	return TRUE;
}


int main(int argc, char* argv[])
{
    try
    {
        SspiInitialize ( );

        // enumerate security packages
        SspiPackageList list;
        SspiPackageList::iterator it;
        for ( it = list.begin ( ); it != list.end ( ); it++ )
        {
            std::cout << *it << std::endl;
        }
        SspiServer  server(_T("NTLM"));
        SspiClient  client(_T("NTLM"), NULL );

        server.AcquireCredentials ( );
        client.AcquireAlternateCredentials ( NULL, _T("laurar"), _T("")  );

        SspiData*  client_data;
        SspiData*  server_data;

        client_data = client.PrepareOutboundPackage ( NULL );
        while ( 1 )
        {
            server_data = server.ProcessInboundPackage ( client_data );
            client.FreeBuffer ( client_data );
            if ( server.GetAuthState ( ) == wsspi::AuthSuccess )
            {
                printf ( "Authenticated!\n" );
                break;
            }
            else if ( server.GetAuthState ( ) == wsspi::AuthFailed )
            {
                printf ( "Error!\n" );
                return 0;
            }
            client.PrepareOutboundPackage ( server_data );
            server.FreeBuffer ( server_data );
        }

        server.Impersonate ( );
        TCHAR   User[200];
        DWORD   size = 200;
        GetUserName ( User, &size );
        std::cout << _T("Now running as: ") << User << std::endl;

        HANDLE hToken;
        BOOL b = OpenThreadToken ( GetCurrentThread ( ), TOKEN_ALL_ACCESS, TRUE,  &hToken );
        server.RevertToSelf ( );
        if ( b )
        {
            TOKEN_TYPE type;
            DWORD len;
            GetTokenInformation ( hToken, TokenType, (void*)&type, sizeof type, &len );
            printf ( "Token Type: %d\n", type );

            SECURITY_IMPERSONATION_LEVEL level;
            GetTokenInformation ( hToken, TokenImpersonationLevel, (void*)&level, sizeof level, &len );
            printf ( "Impersonation Level: %d\n", level );

            CloseHandle ( hToken );

        }
        else printf ( "Error %lu\n", GetLastError ( ) );

    }
    catch ( SspiException * e )
    {
        std::cout << *e;
        e->Release ( );
    }
	return 0;
}
