//==============================================================================
// File:		SspiServer.cpp
//
// Description:	SspiServer class implementation. This acts as an authentication 
//              server
//
// Revisions: 	Created: 11/22/1999
//
//==============================================================================
// Copyright(C) 1999, Tomas Restrepo. All rights reserved
//==============================================================================
// SspiServer.cpp: implementation of the SspiServer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "..\inc\SspiDef.h"
#include "..\inc\SspiPackage.h"
#include "..\inc\SspiServer.h"

using namespace wsspi;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SspiServer::SspiServer ( const SspiPackage & package )
    : m_Package ( package ),
      m_HaveContext ( false ),
      m_State ( AuthContinue )
{
}

SspiServer::~SspiServer ( )
{
    if ( m_HaveContext )
        _pSecurityInterface->DeleteSecurityContext ( &m_Context );
}

//==========================================================
// SspiServer::FreeBuffer()
//	Description
//      releases the memory allocated for an SspiData
//      allocated by ProcessInboundPackage()
//		
//	Parameters
//		Data		 -  data buffer 
//	Return
//		void		 - none
//==========================================================
void SspiServer::FreeBuffer(SspiData* &data) const
{
	if (data != NULL) {
		delete[] (BYTE*) data;
		data = NULL;
	}
}

//==========================================================
// SspiServer::AcquireCredentials()
//	Description
//		Acquires credentials based on the current logon id
//		
//	Parameters
//	Return
//		void		 - 
//==========================================================
void SspiServer::AcquireCredentials ( )
{
    TimeStamp           Expiration;
    SECURITY_STATUS     status;

    status = _pSecurityInterface->AcquireCredentialsHandle ( 
                            NULL,
                            m_Package->Name,
                            SECPKG_CRED_INBOUND,
                            NULL, NULL,
                            NULL, NULL,
                            &m_Credentials,
                            &Expiration
                        );
    if ( status != SEC_E_OK )
        THROWEXE ( ErrorNoCredentials, status );
}

//==========================================================
// SspiServer::ProcessInboundPackage()
//	Description
//		
//	Parameters
//		pInbound		 - Inbound data from client
//	Return
//		SspiData*		 - outbound data to client
//==========================================================
SspiData * SspiServer::ProcessInboundPackage ( 
                                SspiData * pInbound 
                            )
{
    SecBufferDesc   ibd, obd;
    SecBuffer       ib,  ob;
    SECURITY_STATUS status;
    SspiData*       pOutbound;        

    // inbound data cannot be NULL
    if ( pInbound == NULL )
        THROWEX ( ErrorNullPointer );

    // prepare inbound buffer
    ib.BufferType = SECBUFFER_TOKEN;
    ib.cbBuffer   = pInbound->Size;
    ib.pvBuffer   = pInbound->Buffer;
    // prepare buffer description
    ibd.cBuffers  = 1;
    ibd.ulVersion = SECBUFFER_VERSION;
    ibd.pBuffers  = &ib;

    // prepare outbound buffer
    ob.BufferType = SECBUFFER_TOKEN;
    ob.cbBuffer   = m_Package->cbMaxToken;
    pOutbound     = reinterpret_cast<SspiData*>(new BYTE[ob.cbBuffer + sizeof(DWORD)]);
    if ( pOutbound == NULL )
        THROWEX ( ErrorNoMemory );
    ob.pvBuffer   = pOutbound->Buffer;
    // prepare buffer description
    obd.cBuffers  = 1;
    obd.ulVersion = SECBUFFER_VERSION;
    obd.pBuffers  = &ob;

    // try to accept the clients token
    DWORD      CtxtAttr;
    TimeStamp  Expiration;

    status = _pSecurityInterface->AcceptSecurityContext ( 
                            &m_Credentials,
                            m_HaveContext ? &m_Context : NULL,
                            &ibd,
                            ASC_REQ_REPLAY_DETECT | ASC_REQ_SEQUENCE_DETECT |
                            ASC_REQ_CONFIDENTIALITY | ASC_REQ_DELEGATE, 
                            SECURITY_NATIVE_DREP,
                            &m_Context,
                            &obd,
                            &CtxtAttr,
                            &Expiration
                        );
    
    if ( (status == SEC_I_COMPLETE_NEEDED) ||
         (status == SEC_I_COMPLETE_AND_CONTINUE) )
    {
        if ( _pSecurityInterface->CompleteAuthToken != NULL )
            _pSecurityInterface->CompleteAuthToken ( &m_Context, &obd );
    }

    switch ( status )
    {
    case SEC_E_OK:
    case SEC_I_COMPLETE_NEEDED:
        m_State = AuthSuccess;   // we're done here
        break;
    case SEC_I_CONTINUE_NEEDED:
    case SEC_I_COMPLETE_AND_CONTINUE:
        m_State = AuthContinue;  // keep on going
        break;
    case SEC_E_LOGON_DENIED:
        m_State = AuthFailed;    // logon denied
        break;
    default:
        m_State = AuthError;
        // make sure we don't leak memory
        FreeBuffer ( pOutbound );
        THROWEXE ( ErrorAuthFailed, status );
    }

    // we should now have a context
    m_HaveContext = true;

    // adjust the size, as ISC() might have changed it:
    pOutbound->Size = ob.cbBuffer;

    return pOutbound;
}

//==========================================================
// SspiServer::Impersonate()
//	Description
//		Impersonates the client's security context
//      Keep in mind this only gives us an impersonation
//      token (a.k.a. a network logon)
//
//	Parameters
//	Return
//		void		 - none
//==========================================================
void SspiServer::Impersonate ( )
{
    SECURITY_STATUS status = _pSecurityInterface->ImpersonateSecurityContext ( &m_Context );
    if ( status != SEC_E_OK )
        THROWEXE ( ErrorImpersonationFailed, status );
}

//==========================================================
// SspiServer::RevertToSelf()
//	Description
//		Reverts to the server's own security context
//
//	Parameters
//	Return
//		void		 - 
//==========================================================
void SspiServer::RevertToSelf ( )
{
    SECURITY_STATUS status = _pSecurityInterface->RevertSecurityContext ( &m_Context );
    if ( status != SEC_E_OK )
        THROWEXE ( ErrorRevertToSelfFailed, status );

}
