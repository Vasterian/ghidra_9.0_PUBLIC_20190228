// $Id: NTSystem.cpp,v 1.8 2003/02/12 14:21:19 andy Exp $
// Defines the entry point for the DLL application.
// $Revision: 1.8 $

#define UNICODE
#define SECURITY_WIN32
#define VERSION 101		// 1.01

#include <windows.h>
#include <lmcons.h>     // DNLEN, UNLEN
#include <tchar.h>
#include <stdio.h>
#include <iostream>

#include "security.h"
#include "wsspi.h"
#include "NTSystem.h"

/* Macro to make function definitions a little less verbose.
 */
#define n(f) \
	Java_com_tagish_auth_win32_NTSystem_ ## f

/* Per instance workspace. A pointer to this workspace is stored
 * as an int in the Java proxy class.
 */
struct workspace
{
	/* All the workspace chunks are linked together so we can discard them
	 * all when the DLL is unloaded.
	 */
	workspace	*pLink;

	/* Thread token for the current user. This starts out as the current
	 * user for the machine, but is updated if logon() is called.
	 */
	HANDLE		hUserToken;

	/* Buffer that gets passed to QueryToken(). After a call to QueryToken() this
	 * buffer will contain the returned information.
	 */
	PUCHAR		pInfoBuffer;
	DWORD		dwInfoBufferSize;

	/* String buffer that gets used for textual SID representations.
	 */
	LPTSTR		pTextualSID;
	DWORD		dwTextualSIDSize;
};

static workspace	*pWorld = 0;
static int			usage	= 0;

/* Forward declarations */

static void FreeAll();

using namespace wsspi;

/* Main entry point for DLL. The main thing we do in here is keep track of
 * how many instances of the DLL are extant so we can initialise wsspi when
 * the first instance of the DLL is created and dispose of it when the last
 * instance dies.
 */
BOOL APIENTRY DllMain(HANDLE hModule, DWORD  dwReason, LPVOID lpReserved)
{
	(void) hModule;
	(void) lpReserved;

	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		if (usage++ == 0) {
			SspiInitialize();		// Initialise the wsspi library
		}
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		if (--usage == 0) {
			FreeAll();				// Free all the workspace
			SspiUninitialize();		// Close down the wsspi library
		}
		break;
	}
    return TRUE;
}

/* Close the stashed process token.
 */
static void CloseUserToken(workspace *pWS)
{
	if (pWS->hUserToken != NULL) {
		CloseHandle(pWS->hUserToken);
		pWS->hUserToken = NULL;
	}
}

/* Stash the user token for the current user in the
 * workspace.
 */
static void GetCurrentUser(workspace *pWS)
{
	CloseUserToken(pWS);		// close any existing token

	// Have a look for a thread token.
	if (OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &pWS->hUserToken))
		return;

	// If that token wasn't available get the process token.
	if (GetLastError() == ERROR_NO_TOKEN)
		OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &pWS->hUserToken);

	// Otherwise exit leaving the token null
}

/* Return a pointer to our allocated workspace
 */
static workspace *GetWorkspace(JNIEnv *env, jobject __this)
{
	jfieldID _mem = env->GetFieldID(env->GetObjectClass(__this), "_mem", "I");
	if (_mem == 0) return 0;
	return (workspace *) env->GetIntField(__this, _mem);
}

/* Set the value of the workspace pointer. The pointer is actually
 * an int in the proxy class.
 */
static void SetWorkspace(JNIEnv *env, jobject __this, workspace *pWS)
{
	jfieldID _mem = env->GetFieldID(env->GetObjectClass(__this), "_mem", "I");
	//if (_mem == 0) return; -- don't do this because an exception is better than silently ignoring the problem
	env->SetIntField(__this, _mem, (jint) pWS);
}

/* Return a pointer to our workspace initialising it if necessary.
 * The workspace is lazy-allocated the first time it is needed.
 */
static struct workspace *FindWorkspace(JNIEnv *env, jobject __this)
{
	workspace *pWS = GetWorkspace(env, __this);
	if (0 == pWS) {
		pWS = new workspace;
		memset(pWS, 0, sizeof(*pWS));

		// Link to head of chain
		pWS->pLink = pWorld;
		pWorld = pWS;

		// Perform workspace initialisation
		GetCurrentUser(pWS);

		// Stash the workspace pointer for later
		SetWorkspace(env, __this, pWS);
		//printf("Workspace %p allocated\n", pWS);
	}
	return pWS;
}

/* Recursively take a workspace chunk out of a linked list.
 * To remove pWS from a list at pList do
 *    pList = UnlinkWorkspace(pList, pWS);
 */
static workspace *UnlinkWorkspace(workspace *pBase, workspace *pWS)
{
	if (pBase == 0) return 0;
	if (pBase == pWS) return pBase->pLink;
	pBase->pLink = UnlinkWorkspace(pBase->pLink, pWS);
	return pBase;
}

/* Indicate whether the specified workspace chunk is in the
 * linked list. We need to know this so we don't ever try
 * to free the same chunk twice.
 */
static BOOL IsInList(workspace *pWS)
{
	workspace *pThis;
	if (0 == pWS)
		return FALSE;
	for (pThis = pWorld; pThis != 0; pThis = pThis->pLink)
		if (pThis == pWS)
			return TRUE;
	return FALSE;
}

/* Free a workspace chunk and any additional storage and resources
 * it refers to. If you add anything to workspace that needs to
 * be explicitly disposed of this is where it should happen.
 */
static void FreeWorkspace(workspace *pWS)
{
	if (IsInList(pWS)) {
		// Remove from chain
		pWorld = UnlinkWorkspace(pWorld, pWS);
		CloseUserToken(pWS);
		delete pWS->pInfoBuffer;
		delete pWS->pTextualSID;
		delete pWS;
		//printf("Workspace %p deleted\n", pWS);
	}
}

/* Free all workspace chunks. This call is made when the DLL
 * is closing down to make sure everything gets cleaned up.
 */
static void FreeAll()
{
	while (pWorld != 0)
		FreeWorkspace(pWorld);
}

#if 0
/* Debugging support. Output the contents of a chunk of memory
 * in hex and ascii.
 */
static void hexdump(void *base, size_t length)
{
	unsigned char *mem = (unsigned char *) base;
	unsigned long pos = 0;
	unsigned long ofs;
	while (pos < length)
	{
		printf("%08lx : ", pos);
		for (ofs = 0; ofs < 16 && ofs + pos < length; ofs++)
			printf("%02lx ", mem[pos + ofs]);
		for (; ofs < 16; ofs++)
			printf("   ");
		for (ofs = 0; ofs < 16 && ofs + pos < length; ofs++) {
			unsigned char c = mem[pos + ofs];
			if (c >= ' ' && c != 0x7F)
				putchar(c);
			else
				putchar('.');
		}
		putchar('\n');
		pos += 16;
	}
}
#endif

/* Get information from the current user token and store it
 * in a buffer in the workspace. If the buffer is too small
 * it will be grown to suit.
 */
static PUCHAR QueryToken(workspace *pWS, TOKEN_INFORMATION_CLASS tic)
{
	//printf("token: %ld\n", (long) hToken);
	DWORD	dwNeedSize;
	BOOL	bSuccess;

	// First try
	bSuccess = GetTokenInformation(pWS->hUserToken, tic, pWS->pInfoBuffer, pWS->dwInfoBufferSize, &dwNeedSize);
	if (!bSuccess) {
		// Find out whether the attempt failed because the buffer wasn't big
		// enough. If so grow the buffer to the required size and try again.
		if (ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
			// Need to grow our buffer
			delete pWS->pInfoBuffer;
			pWS->pInfoBuffer = new UCHAR[dwNeedSize];
			pWS->dwInfoBufferSize = dwNeedSize;
			bSuccess = GetTokenInformation(pWS->hUserToken, tic, pWS->pInfoBuffer, pWS->dwInfoBufferSize, &dwNeedSize);
		}
	}

	return bSuccess ? pWS->pInfoBuffer : 0;
}

/* Get the user and domain names from the current user token. Evidently
 * it's OK for the buffers for UserName and Domain to be fixed size.
 */
static BOOL GetUserAndDomainName(workspace *pWS, LPTSTR UserName, LPDWORD cchUserName, LPTSTR DomainName, LPDWORD cchDomainName)
{
	SID_NAME_USE	snu;
	PUCHAR			pInfoBuffer;

	if (pInfoBuffer = QueryToken(pWS, TokenUser), pInfoBuffer == 0) return FALSE;
	return LookupAccountSid(NULL, ((PTOKEN_USER) pInfoBuffer)->User.Sid, UserName,
								cchUserName, DomainName, cchDomainName, &snu);
}

/* Convert a binary SID into its textual representation. This code comes
 * via minor modifications from a Microsoft example. The buffer for the
 * text is within the workspace and will be grown to suit.
 *
 * If bDomainOnly is TRUE the last element of the SID will be omitted.
 * Apparently the last part of the SID is always the user ID.
 */
static LPTSTR GetTextualSid(workspace *pWS, PSID pSid, BOOL bDomainOnly, PDWORD pdwLength)
{
	PSID_IDENTIFIER_AUTHORITY	psia;
	DWORD						dwSubAuthorities;
	DWORD						dwSidRev = SID_REVISION;
	DWORD						dwCounter;
	DWORD						dwSidSize;

	// Validate the binary SID.
	if (!IsValidSid(pSid)) return 0;

	// Get the identifier authority value from the SID.
	psia = GetSidIdentifierAuthority(pSid);

	// Get the number of subauthorities in the SID.
	dwSubAuthorities = *GetSidSubAuthorityCount(pSid);

	// Compute the buffer length.
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL
	dwSidSize = (15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);

	if (dwSidSize > pWS->dwTextualSIDSize) {
		delete pWS->pTextualSID;
		pWS->pTextualSID = new TCHAR[dwSidSize];
		pWS->dwTextualSIDSize = dwSidSize;
	}

	// Add 'S' prefix and revision number to the string.
	dwSidSize = wsprintf(pWS->pTextualSID, TEXT("S-%lu-"), dwSidRev);

	// Add SID identifier authority to the string.
	if ((psia->Value[0] != 0) || (psia->Value[1] != 0)) {
		dwSidSize += wsprintf(pWS->pTextualSID + dwSidSize,
								TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),
								(USHORT) psia->Value[0], (USHORT) psia->Value[1],
								(USHORT) psia->Value[2], (USHORT) psia->Value[3],
								(USHORT) psia->Value[4], (USHORT) psia->Value[5]);
	} else {
		dwSidSize += wsprintf(pWS->pTextualSID + dwSidSize,
								TEXT("%lu"),
								(ULONG) (psia->Value[5]    )   +
								(ULONG) (psia->Value[4] <<  8)   +
								(ULONG) (psia->Value[3] << 16)   +
								(ULONG) (psia->Value[2] << 24)  );
	}

	if (bDomainOnly) dwSubAuthorities--;

	// Add SID subauthorities to the string.
	for (dwCounter = 0; dwCounter < dwSubAuthorities; dwCounter++)
		dwSidSize += wsprintf(pWS->pTextualSID + dwSidSize, TEXT("-%lu"), *GetSidSubAuthority(pSid, dwCounter));

	if (pdwLength != 0) *pdwLength = dwSidSize;

	return pWS->pTextualSID;
}

/* Get a printable SID as a Java string.
 */
static jstring GetJStringSID(JNIEnv *env, workspace *pWS, PSID pSid, BOOL bDomainOnly)
{
	DWORD dwLength = 0;
	TCHAR *pBuffer = GetTextualSid(pWS, pSid, bDomainOnly, &dwLength);
	if (pBuffer == 0) return 0;
	return env->NewString((const jchar *) pBuffer, (jsize) dwLength);
}

/* Get the textual name of a SID as a Java string. If the name isn't available
 * (expired account, PDC can't be contacted) the printable SID will be returned
 * instead.
 */
static jstring GetJStringName(JNIEnv *env, workspace *pWS, PSID pSid, BOOL *pbGotName)
{
	TCHAR			User[UNLEN + 1];
	TCHAR			Domain[DNLEN + 1];
	DWORD			cchUser		= UNLEN + 1;
	DWORD			cchDomain	= DNLEN + 1;
	SID_NAME_USE	snu;
	BOOL			bGotName;

	//printf("About to lookup %p\n", pSid);
	bGotName = LookupAccountSid(NULL, pSid, User, &cchUser, Domain, &cchDomain, &snu);
	if (pbGotName != 0) *pbGotName = bGotName;
	if (!bGotName) {
		//printf("Failed: GetLastError() = %d\n", GetLastError());
		/* Failover to returning the printable representation
		 * of the SID
		 */
		return GetJStringSID(env, pWS, pSid, FALSE);
	}
	//printf("Lookup successful\n");
    return env->NewString((const jchar *) User, (jsize) cchUser);
}

/* Copy a string removing leading and trailing whitespace and converting
 * multiple whitespace characters within the string to a single space.
 * This is used to turn Win32 error messages (which typically end with
 * CRLF) into Java exception messages.
 */
static void Sweeten(LPTSTR lpOut, LPTSTR lpIn)
{
	// Skip leading whitespace
	while (*lpIn != (TCHAR) '\0' && *lpIn <= (TCHAR) ' ')
		lpIn++;
	int iRunLen = 0;	// length of a run of whitespace
	while (*lpIn != (TCHAR) '\0') {
		if (*lpIn <= (TCHAR) ' ') {
			// If it's whitespace just count it
			iRunLen++;
			lpIn++;
		} else {
			if (iRunLen > 0) {
				// Flush any pending pending whitespace with
				// a single space.
				*lpOut++ = (TCHAR) ' ';
				iRunLen = 0;
			}
			*lpOut++ = *lpIn++;
		}
	}
	*lpOut = (TCHAR) '\0';
}

/* Convert a Win32 error code into an error message. The returned string
 * must be disposed of by calling LocalFree().
 */
static LPTSTR GetErrorMessage(DWORD dwLastError)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				    NULL, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

	Sweeten((LPTSTR) lpMsgBuf, (LPTSTR) lpMsgBuf);
	// hexdump(lpMsgBuf, 2 * wcslen((LPTSTR) lpMsgBuf));

	return (LPTSTR) lpMsgBuf;
}

/* Look up the class for an exception defaulting to
 * java.lang.Exception if the desired class can't be
 * found.
 */
static jclass FindException(JNIEnv *env, const char *name)
{
	// Find the base exception first, because FindClass stops being
	// able to find anything once it has failed. Doh!
	jclass e  = env->FindClass("java/lang/Exception");
	jclass ex = env->FindClass(name);
	return ex == 0 ? e : ex;

}

/* Throw a Java exception with a message looked up from a Win32
 * error code.
 */
static jint ThrowWin32Error(JNIEnv *env, DWORD dwLastError, const char *clazz)
{
	LPTSTR		lpMessage	= GetErrorMessage(dwLastError);
	jclass		jcExc		= FindException(env, clazz);
	jmethodID	jmInit		= env->GetMethodID(jcExc, "<init>", "(Ljava/lang/String;)V");
	jstring		jsMsg		= env->NewString(lpMessage, wcslen(lpMessage));
	jobject		joExc		= env->NewObject(jcExc, jmInit, jsMsg);
	LocalFree(lpMessage);
	return env->Throw((jthrowable) joExc);
}

JNIEXPORT jint JNICALL n(getVersion)(JNIEnv *env, jobject __this)
{
	return VERSION;
}


/* Clean up NT resources during garbage collection.
 */
JNIEXPORT void JNICALL n(finalize)(JNIEnv *env, jobject __this)
{
	/* Free the workspace. This call is safe even if the workspace has
	 * already been freed by some other means because FreeWorkspace()
	 * checks that the specified workspace is in the list before
	 * discarding it.
	 */
	workspace *pWS = GetWorkspace(env, __this);
	FreeWorkspace(pWS);
	SetWorkspace(env, __this, 0);
}

/* Get the username for the current or logged on NT user.
 */
JNIEXPORT jstring JNICALL n(getName)(JNIEnv *env, jobject __this)
{
	TCHAR User[UNLEN + 1];
	TCHAR Domain[DNLEN + 1];
	DWORD cchUser = UNLEN;
	DWORD cchDomain = DNLEN;
	workspace		*pWS = FindWorkspace(env, __this);

	if (!GetUserAndDomainName(pWS, User, &cchUser, Domain, &cchDomain)) return 0;
    return env->NewString((const jchar *) User, (jsize) cchUser);
}

/* Get the domain for the current or logged on NT user.
 */
JNIEXPORT jstring JNICALL n(getDomain)(JNIEnv *env, jobject __this)
{
	TCHAR User[UNLEN + 1];
	TCHAR Domain[DNLEN + 1];
	DWORD cchUser = UNLEN;
	DWORD cchDomain = DNLEN;
	workspace		*pWS = FindWorkspace(env, __this);

	if (!GetUserAndDomainName(pWS, User, &cchUser, Domain, &cchDomain)) return 0;
    return env->NewString((const jchar *) Domain, (jsize) cchDomain);
}

/* Get a printable SID for the current or logged on NT user's domain.
 */
JNIEXPORT jstring JNICALL n(getDomainSID)(JNIEnv *env, jobject __this)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;
	if (pInfoBuffer = QueryToken(pWS, TokenUser), pInfoBuffer == 0) return 0;
	return GetJStringSID(env, pWS, ((PTOKEN_USER) pInfoBuffer)->User.Sid, TRUE);
}

/* Get the printable group SIDs for the current or logged on NT user.
 */
JNIEXPORT jobjectArray JNICALL n(getGroupIDs)(JNIEnv *env, jobject __this)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;

	if (pInfoBuffer = QueryToken(pWS, TokenGroups), pInfoBuffer == 0) return 0;
	PTOKEN_GROUPS pTG = (PTOKEN_GROUPS) pInfoBuffer;

	jobjectArray jSids = env->NewObjectArray((jsize) pTG->GroupCount, env->FindClass("java/lang/String"), 0);

	for (DWORD dwGroup = 0; dwGroup < pTG->GroupCount; dwGroup++) {
		jstring jsSid = GetJStringSID(env, pWS, pTG->Groups[dwGroup].Sid, FALSE);
		env->SetObjectArrayElement(jSids, (jsize) dwGroup, jsSid);
	}

	return jSids;
}

/* Get the group names for the current or logged NT user.
 */
JNIEXPORT jobjectArray JNICALL n(getGroupNames)(JNIEnv *env, jobject __this, jboolean jbSidIfUnavailable)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;
	BOOL			bAvailable;

	if (pInfoBuffer = QueryToken(pWS, TokenGroups), pInfoBuffer == 0) return 0;
	PTOKEN_GROUPS pTG = (PTOKEN_GROUPS) pInfoBuffer;

	jobjectArray jSids = env->NewObjectArray((jsize) pTG->GroupCount, env->FindClass("java/lang/String"), 0);

	for (DWORD dwGroup = 0; dwGroup < pTG->GroupCount; dwGroup++) {
		jstring jsSid = GetJStringName(env, pWS, pTG->Groups[dwGroup].Sid, &bAvailable);
		if (!bAvailable && !jbSidIfUnavailable) jsSid = 0;
		env->SetObjectArrayElement(jSids, (jsize) dwGroup, jsSid);
	}

	return jSids;
}

/* Get an impersonation token for the current or logged on NT user.
 * This isn't implemented yet. It's only here because this started off as
 * a clone of Sun's example.
 */
JNIEXPORT jint JNICALL n(getImpersonationToken)(JNIEnv *env, jobject __this)
{
	(void) env;
	(void) __this;
	return -1;
}

/* Get a printable primary group SID for the current or logged on NT user.
 */
JNIEXPORT jstring JNICALL n(getPrimaryGroupID)(JNIEnv *env, jobject __this)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;

	if (pInfoBuffer = QueryToken(pWS, TokenPrimaryGroup), pInfoBuffer == 0) return 0;
	return GetJStringSID(env, pWS, ((PTOKEN_PRIMARY_GROUP) pInfoBuffer)->PrimaryGroup, FALSE);
}

/* Get a printable primary group name for the current or logged on NT user.
 */
JNIEXPORT jstring JNICALL n(getPrimaryGroupName)(JNIEnv *env, jobject __this)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;

	if (pInfoBuffer = QueryToken(pWS, TokenPrimaryGroup), pInfoBuffer == 0) return 0;
	return GetJStringName(env, pWS, ((PTOKEN_PRIMARY_GROUP) pInfoBuffer)->PrimaryGroup, 0);
}

/* Get a printable SID for the current or logged on NT user.
 */
JNIEXPORT jstring JNICALL n(getUserSID)(JNIEnv *env, jobject __this)
{
	workspace		*pWS = FindWorkspace(env, __this);
	PUCHAR			pInfoBuffer;

	if (pInfoBuffer = QueryToken(pWS, TokenUser), pInfoBuffer == 0) return 0;
	return GetJStringSID(env, pWS, ((PTOKEN_USER) pInfoBuffer)->User.Sid, FALSE);
}

/* Log the user off. This call returns this object to the state it was
 * in before a call to logon()
 */
JNIEXPORT void JNICALL n(logoff)(JNIEnv *env, jobject __this)
{
	workspace *pWS = FindWorkspace(env, __this);
	GetCurrentUser(pWS);
}

/* Attempt to log a user on. Once the user is logged on other methods
 * in this class will return information about the specified user rather
 * than the current user.
 */
JNIEXPORT void JNICALL n(logon)(JNIEnv *env, jobject __this, jstring jsUserName, jcharArray jsPassword, jstring jsDomain)
{
	static const char *lpcstrAuthFailed = "javax/security/auth/login/FailedLoginException";
	workspace	*pWS = FindWorkspace(env, __this);
	jboolean	isCopy, isPwdCopy;
	HANDLE		hToken;
	const jchar	*jcUserName	= jsUserName ? env->GetStringChars(jsUserName,	&isCopy)			: 0;
	const jchar	*jcDomain	= jsDomain   ? env->GetStringChars(jsDomain,	&isCopy)			: 0;
	jchar		*jcPassword	= jsPassword ? env->GetCharArrayElements(jsPassword, &isPwdCopy)	: 0;
	jsize		jsPwdLen	= jsPassword ? env->GetArrayLength(jsPassword)						: 0;

	// Turn the jcPassword jchar array into a null terminated TCHAR array. We're relying here
	// on sizeof(TCHAR) == sizeof(jchar).
	TCHAR *lpstrPwd = new TCHAR[jsPwdLen + 1];
	if (jcPassword != 0) memcpy(lpstrPwd, jcPassword, jsPwdLen * sizeof(TCHAR));
	lpstrPwd[jsPwdLen] = '\0';

	/* Not sure about this. Does this mean we're always using NTLM? */
    SspiServer server(TEXT("NTLM"));
    SspiClient client(TEXT("NTLM"), NULL);

    SspiData *client_data = NULL;
    SspiData *server_data = NULL;

    try {

        server.AcquireCredentials();
        client.AcquireAlternateCredentials(jcDomain, jcUserName, lpstrPwd);

        client_data = client.PrepareOutboundPackage(NULL);

        for (;;) {
			server.FreeBuffer(server_data);
            server_data = server.ProcessInboundPackage(client_data);
            if (server.GetAuthState() == wsspi::AuthSuccess) {
                break;
            } else if (server.GetAuthState() == wsspi::AuthFailed) {
				env->ThrowNew(FindException(env, lpcstrAuthFailed), "Authentication failed");
				goto done;
            }
            client.FreeBuffer(client_data);
			client_data = client.PrepareOutboundPackage(server_data);
        }
        client.FreeBuffer(client_data);

		server.Impersonate();
		BOOL bOK = OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE,  &hToken);
		server.RevertToSelf();

        if (bOK) {
			CloseUserToken(pWS);
			pWS->hUserToken = hToken;
        } else {
			ThrowWin32Error(env, GetLastError(), lpcstrAuthFailed);
			goto done;
		}

    } catch (SspiException *e) {
		ThrowWin32Error(env, e->GetError(), lpcstrAuthFailed);
        e->Release();
    }

done:
	// We expect all control paths to reach this point...
	client.FreeBuffer(client_data);
	server.FreeBuffer(server_data);

	// Clear any copies of the password we may have
	memset(lpstrPwd, 0, jsPwdLen * sizeof(TCHAR));
	delete lpstrPwd;

	if (jcPassword) {
		if (isPwdCopy) memset(jcPassword, 0, jsPwdLen * sizeof(TCHAR));
		env->ReleaseCharArrayElements(jsPassword, jcPassword, 0);
	}

	if (jcDomain)   env->ReleaseStringChars(jsDomain, jcDomain);
	if (jcUserName)	env->ReleaseStringChars(jsUserName, jcUserName);
}
