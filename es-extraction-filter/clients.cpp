#include "stdafx.h"
#include "es-extraction-filter.h"
#include "MainFrm.h"
#include <process.h> 

// RPC에서 사용되는 메모리 할당과 해제 함수를 정의한다.
void * __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER midl_user_free(void* p)
{
	free(p);
}

unsigned __stdcall AyncWaitThread(void *lp)
{
	CRPCThread *p = (CRPCThread*)lp;
	DWORD	ret = 0;
	int		retp = 0;
	do
	{
		ret = WaitForSingleObject( p->m_pAsync->u.hEvent, 100 );
		if( ret == WAIT_OBJECT_0 )
		{
			RpcAsyncCompleteCall(p->m_pAsync,&retp);
			break;
		}
	}while(p->m_bThreadExit == false);

	if( p->m_bThreadExit == false )
	{
		p->ReadES();
		p->m_pwnd->PostMessage(WM_COMPRESSDONE,0,(LPARAM)p);
	}

	return 0;
}

CRPCThread::CRPCThread()
{
	bindingStr = 0;
	handleBinding = 0;
	m_pwnd = 0;

	m_nIndex = -1;

	m_hThread = INVALID_HANDLE_VALUE;
	m_bThreadExit = false;
	m_pAsync = 0;
}

CRPCThread::~CRPCThread()
{
	m_bThreadExit = true;
	if( m_hThread != INVALID_HANDLE_VALUE )
	{		
		WaitForSingleObject( m_hThread, INFINITE );
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}

	CloseHandle(m_pAsync->u.hEvent);

	if( m_pAsync != 0 )
		delete m_pAsync;

	m_pAsync = 0;

	Close(&m_ctx);

	if( bindingStr != 0 )
		RpcStringFree(&bindingStr);

	if( handleBinding != 0 )
		RpcBindingFree(&handleBinding);
}

void CRPCThread::ReadES(void)
{
	if( handleBinding == 0 )
		return;

	char path[MAX_PATH+2];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(0, path, MAX_PATH);
	_splitpath_s( path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );	

	sprintf_s(es_filename, 256, "%s%s%d.es",drive, dir, m_nIndex);
	HANDLE hLocalFile = CreateFile(es_filename,
									GENERIC_WRITE,
									FILE_SHARE_WRITE,
									0,
									CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL,
									0);
	if( INVALID_HANDLE_VALUE == hLocalFile )
		return;

	//CloseSegFile(handleBinding);
	CloseSegFile(m_ctx);

	//int bOpen = OpenCompressedData(handleBinding);
	int bOpen = OpenCompressedData(m_ctx);
	int read_len = 64*1024;
	PBYTE	pES = (PBYTE)midl_user_allocate(read_len);

	int ret = 0;
	int nRead = 0;
	__int64 total_len, len = 0;
	DWORD	dwWritten = 0;

	//ret = GetCompressedDataInfo(handleBinding, &total_len);
	ret = GetCompressedDataInfo(m_ctx, &total_len);
	if( ret == 1 && total_len > 0 )
	{
		while( total_len > len )
		{
			//ret = GetCompressedData(handleBinding,
			ret = GetCompressedData(m_ctx,
									read_len,
									&nRead,
									pES);
			if( ret == 0 || nRead == 0 )
			{
				break;
			}

			dwWritten = 0;
			WriteFile( hLocalFile, pES, nRead, &dwWritten, 0 );
			len += nRead;
		}
	}
	//
	midl_user_free(pES);

	RpcTryExcept 
	{
		//CloseCompressedData(handleBinding);
		CloseCompressedData(m_ctx);		
	}
	RpcExcept(1)
	{
	}
	RpcEndExcept

	CloseHandle(hLocalFile);
	hLocalFile = INVALID_HANDLE_VALUE;
}

void CRPCThread::EncodeStart(void)
{
	//DoEncode(handleBinding);
	RpcTryExcept
	{
		m_hThread = (HANDLE)_beginthreadex(	0,
											0,
											AyncWaitThread,
											this,
											0,
											0);
		//AsyncFunc( m_pAsync, handleBinding, DEFAULT_ASYNC_DELAY);
		Sleep(0);
		//AsyncFunc( m_pAsync, handleBinding);
		AsyncFunc( m_pAsync, m_ctx);
	}
	RpcExcept(1)
	{
		unsigned long ulCode = RpcExceptionCode();
		char str[256];
		sprintf_s(str,256,"Asyncfunc : run time reported exception 0x%lx",ulCode);
		OutputDebugStringA(str);
	}
	RpcEndExcept
}

void CRPCThread::OpenRemoteFile(int index)
{
	m_nIndex = index;
	///OpenNewSegFile(handleBinding,m_nIndex);
	OpenNewSegFile(m_ctx,m_nIndex);
}

bool CRPCThread::TryRPCConnet(void)
{
	bool tf = TryRPCConnet(
						(RPC_STR*)	"ncacn_ip_tcp",
						(RPC_STR*)	m_szIP.GetBuffer(),
						(RPC_STR*)	m_nPort.GetBuffer()
						);
	if( tf == true )
	{
		if( m_pAsync == 0 )
		{
			m_pAsync = new RPC_ASYNC_STATE;
			RpcAsyncInitializeHandle(m_pAsync, sizeof(RPC_ASYNC_STATE));
			m_pAsync->UserInfo = 0;
			m_pAsync->u.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			m_pAsync->NotificationType = RpcNotificationTypeEvent;
			//m_pAsync->NotificationType = RpcNotificationTypeCallback;

			// http://msdn.microsoft.com/en-us/library/windows/desktop/aa373973(v=vs.85).aspx
		}
	}

	return tf;
}

bool CRPCThread::TryRPCConnet(	_In_opt_ RPC_CSTR ProtSeq,
								_In_opt_ RPC_CSTR NetworkAddr,
								_In_opt_ RPC_CSTR Endpoint)
{
	// 바인딩 문자열을 얻어온다.
	RPC_STATUS status = RpcStringBindingCompose(
										NULL,
										//(RPC_STR*)"ncacn_ip_tcp",	// TCP/IP 프로토콜을 사용
										ProtSeq,
										//(RPC_STR*)"localhost",		// TCP/IP로 접속할 RPC 서버의 IP 주소
										NetworkAddr,
										//(RPC_STR*)"5050",			// TCP/IP로 접속할 RPC 서버의 포트
										Endpoint,
										NULL,
										&bindingStr);				// 바인딩 문자열
	if (status != RPC_S_OK)
	{
		TRACE("RpcStringBindingCompose() error \n");
		return false;
	}

	EPT_S_INVALID_ENTRY;

	// 바인딩 문자열로부터 바인딩 핸들을 얻어온다.
	status = RpcBindingFromStringBinding(
									bindingStr,				// 바인딩 문자열
									&handleBinding);			// 바인딩 핸들
	if (status != RPC_S_OK)
	{
		TRACE("RpcBindingFromStringBinding() error\n");
		return false;
	}

	status = RpcEpResolveBinding(handleBinding, InterfaceName_v1_0_c_ifspec);

	RpcTryExcept 
	{
		// 여기서 RPC 서버의 함수를 호출한다.
		m_ctx = Connect(handleBinding, (unsigned char*)"Hello World!");
	}
	RpcExcept(1)
	{
		TRACE("Runtime reported exception %d \n", RpcExceptionCode());
	}
	RpcEndExcept

	return true;
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/aa373550(v=vs.85).aspx
