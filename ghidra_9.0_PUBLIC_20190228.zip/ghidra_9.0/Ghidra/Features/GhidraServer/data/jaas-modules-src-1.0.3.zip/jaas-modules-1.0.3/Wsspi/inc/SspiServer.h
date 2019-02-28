//==============================================================================
// File:		SspiServer.h
//
// Description:	SspiServer class declaration
//
// Revisions: 	Created: 11/22/1999
//
//==============================================================================
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//==============================================================================

#if !defined(_SSPISERVER_H_)
#define _SSPISERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
namespace wsspi
{
    class SspiServer
    {
    public:
        SspiServer ( const SspiPackage & package );
	    virtual ~SspiServer ( );

        virtual void AcquireCredentials ( );
        virtual SspiData * ProcessInboundPackage ( SspiData * pInbound );
        virtual void Impersonate ( );
        virtual void RevertToSelf ( );

        // -- buffer management --
        void FreeBuffer ( SspiData* &data ) const;

        // -- accessors --
        AuthState GetAuthState ( ) { return m_State; }

        // -- private data --
    private:
        // package info
        SspiPackage   m_Package;
        // credentials
        CredHandle    m_Credentials;
        // context
        CtxtHandle    m_Context;
        // do we have a context yet?
        bool          m_HaveContext;
        // state of the authentication
        AuthState     m_State;
    };
}
#endif // _SSPISERVER_H
