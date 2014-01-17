// es_rpc_server.cpp : Defines the entry point for the console application.
//

// ref. : http://www.codeproject.com/Articles/4878/Introduction-to-RPC-Part-2


#include "stdafx.h"
#include "streams.h"
#include <tchar.h>
#include <process.h>
#include <atlcomcli.h>

#include "interface_h.h"
#include "FrameInfo.h"
#include "ESSourceFilter.h"
#include "dshowutil.h"
#include "moreuuids.h"
#include <initguid.h>
#include "NullRenderer.h"
#include "IAMGB.h"

#include <hash_map>

using namespace stdext;

class TCTX
{
public:
	TCTX()
	{
		m_Handle = 0;
	}
	UINT m_Handle;
};

#ifdef UNICODE
	#define RPC_STR		RPC_WSTR
#else
	#define RPC_STR		RPC_CSTR
#endif

DEFINE_GUID(CLSID_FFDSHOW, 
	0x04FE9017, 0xF873, 0x410E, 0x87, 0x1E, 0xAB, 0x91, 0x66, 0x1A, 0x4E, 0xF7);

//{04FE9017-F873-410E-871E-AB91661A4EF7}

void __cdecl ThreadProc(void *);

list<CTransServer*> SrvList;
hash_map<handle_t, CTransServer*> handle_map;


// RPC에서 사용되는 메모리 할당과 해제 함수를 정의한다.
void * __RPC_USER midl_user_allocate(size_t size)
{
	return malloc(size);
}

void __RPC_USER midl_user_free(void* p)
{
	free(p);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//sprintf_s(filename,256,"local_&&_0.es");
	//videoType = MEDIASUBTYPE_AVC1;
	//AvgTimePerFrame = 400000LL;
	//gVideoWidth = 1280;
	//gVideoHeight = 720;
	//ThreadProc(0);

	//char x264_options[1024];
	//sprintf_s(x264_options, 1024, "x264-r2377-1ca7bb9.exe --input-depth 8 --input-res %dx%d --intra-refresh -r 2 -q 26 --b-adapt 0 --no-scenecut --keyint 15 -i 15 --bframes 0 -o %s _target_.yuv",
	//				gVideoWidth, gVideoHeight, compress_filename);

	//STARTUPINFO StartupInfo = {0,};    //구조체 선언, 초기화
	//PROCESS_INFORMATION ProcessInfo;

	//StartupInfo.cb = sizeof(STARTUPINFO);
	////StartupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USEPOSITION | STARTF_USESIZE;
	//StartupInfo.dwFlags = STARTF_USESHOWWINDOW;

	////Flags 값들이 여러가지가 있습니다.
	////STARTF_USEPOSITION :: dwX, dwY 위치 사용
	////STARTF_USESIZE     :: dwXSize, dwYSize 사용
	////Flags 값이 주어지지 않는 si구조체 값은
	////새로 만들어지는 프로세스에 영향을 주지 않습니다.
	//StartupInfo.dwX = 100;
	//StartupInfo.dwY = 100;
	//StartupInfo.dwXSize = 300;
	//StartupInfo.dwYSize = 300;  //dw ~ 는 사실 잘 쓰이지 않습니다.

	//StartupInfo.wShowWindow = SW_HIDE;

	//StartupInfo.lpTitle = _T(" Child process! ");

	//BOOL tf = CreateProcess( NULL,
	//						x264_options,
	//						0,	// security attribute
	//						0,	// thread attribute
	//						FALSE,	// inherit handle
	//						CREATE_NEW_CONSOLE | CREATE_NO_WINDOW,	// dwCreationFlags
	//						0,	// environment
	//						0,	// current directory
	//						&StartupInfo,
	//						&ProcessInfo);
	//if( tf == TRUE )
	//{
	//	WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
	//	CloseHandle( ProcessInfo.hThread);
	//	CloseHandle( ProcessInfo.hProcess);
	//}

	CoInitialize(0);

	printf("server run\n");

	//hLocalFile = CreateFile(_T("local_&&_0.es"),
	//						GENERIC_READ,
	//						FILE_SHARE_READ,
	//						0,
	//						OPEN_EXISTING,
	//						FILE_ATTRIBUTE_NORMAL,
	//						0);

	// 원격함수호출에 대한 프로토콜을 설정한다.
	RPC_STATUS status = RpcServerUseProtseqEp(
									(RPC_STR)"ncacn_ip_tcp",			// TCP/IP 프로토콜 설정
									RPC_C_PROTSEQ_MAX_REQS_DEFAULT,
									(RPC_STR)DEFAULT_ENDPOINT,			// TCP/IP 포트 번호 설정
									NULL); 

	if (status != RPC_S_OK)
	{
		printf("RpcServerUseProtseqEp() 실패 \n");

		CoUninitialize();
		return -1;
	}

	// IDL 인터페이스를 등록한다.
	// interface_h.h 파일에 등록할 인터페이스가 선언되어 있다.
	//status = RpcServerRegisterIf(InterfaceName_v1_0_s_ifspec, NULL, NULL);
	//if (status != RPC_S_OK) {
	//	printf ("RpcServerRegisterIf() 실패 \n");
	//	return -1;
	//}

	status = RpcServerRegisterIfEx(   
							InterfaceName_v1_0_s_ifspec, // Interface to register.    
							NULL,   
							NULL, // Use the MIDL generated entry-point vector.    
							RPC_IF_ALLOW_CALLBACKS_WITH_NO_AUTH,   
							-1,   
							NULL);

	// 여기서부터 클라이언트로부터의 원격함수호출을 대기한다.
	status = RpcServerListen(
							1,		// 서버에서 만들어지는 쓰레드의 최소 수
							RPC_C_LISTEN_MAX_CALLS_DEFAULT, // 권장되는 쓰레드의 최대 수
							FALSE);	// RpcMgmtStopServerListening () 함수를 호출할 때까지 대기하도록 설정

	if (status != RPC_S_OK)
	{
		printf ("RpcServerListen() 실패 \n");

		CoUninitialize();
		return -1;
	}

	handle_map.clear();

	list <CTransServer*>::size_type SrvCnt = SrvList.size();
	if( SrvCnt > 0 )
	{
		list<CTransServer*>::iterator iter = SrvList.begin();
		while(iter != SrvList.end())
		{
			CTransServer* p = *iter;
			iter++;
			delete p;
		}
		SrvList.clear();
	}

	CoUninitialize();

	return 0;
}

void Close( 
    /* [out][in] */ TRANS_HANDLE *ppTr)
{
	if( *ppTr == 0 )
		return;

	delete *ppTr;
	*ppTr = 0;
}

void __RPC_USER TRANS_HANDLE_rundown( TRANS_HANDLE tr)
{
	Close(&tr);
}

// IDL에서 선언한 서버 함수를 정의한다.
TRANS_HANDLE Connect( 
    /* [in] */ handle_t IDL_handle,
    /* [string][in] */ unsigned char *szOutput)
{
	TCTX *tCTX = new TCTX;
	TRANS_HANDLE tr = (TRANS_HANDLE)tCTX;
	IDL_handle = tr;
	CTransServer *p = new CTransServer;
	handle_map.insert(hash_map<handle_t, CTransServer*>::value_type(tCTX, p));
	SrvList.push_back(p);

	printf("[0x%08x] : %s \n", tr, szOutput);

	return tr;
}

int SetPicturesData(
    /* [in] */ TRANS_HANDLE hTr,
    /* [in][size_is] */ unsigned char pData[],
    /* [in] */ int numbers)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		return 0;
		//p = new CTransServer;
		//handle_map.insert(hash_map<handle_t, CTransServer*>::value_type(IDL_handle, p));
		//SrvList.push_back(p);
	}
	else
	{
		p = findIter->second;
	}

	if( p->hLocalFile != INVALID_HANDLE_VALUE )
	{
		DWORD dwWritten = 0;
		WriteFile(p->hLocalFile, &numbers, 4, &dwWritten,0);
		WriteFile(p->hLocalFile, pData, numbers, &dwWritten,0);
		return 1;
	}

	return 0;
}

int SetVideoType( 
	/* [in] */ TRANS_HANDLE hTr,
    /* [full][in] */ const VIDEO_MEDIA_TYPE *_guid,
    /* [in] */ __int64 fps,
    /* [in] */ int width,
    /* [in] */ int height)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		//p = new CTransServer;
		//handle_map.insert(hash_map<handle_t, CTransServer*>::value_type(IDL_handle, p));
		//SrvList.push_back(p);
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	p->m_VideoWidth = width;
	p->m_VideoHeight = height;
	p->AvgTimePerFrame = fps; 
	memcpy(&p->videoType, _guid, sizeof(VIDEO_MEDIA_TYPE));
	return 1;
}

int OpenNewSegFile( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ int index)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	if( p->hLocalFile != INVALID_HANDLE_VALUE )
		CloseHandle(p->hLocalFile);

	char path[MAX_PATH+2];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(0, path, MAX_PATH);
	_splitpath_s( path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );

	sprintf_s(p->compress_filename, 256, "%s%s_target_comp_%d.264",drive, dir, index);

	SYSTEMTIME st;
	GetSystemTime(&st);

	sprintf_s(p->filename,256,"%s%slocal_&&_%d_%d%02d%02d%02d%02d%02d%03d.es",
												drive, dir, index,
														st.wYear,
														st.wMonth,
														st.wDay,
														st.wHour,
														st.wMinute,
														st.wSecond,
														st.wMilliseconds);
	p->hLocalFile = CreateFile(p->filename,
							GENERIC_WRITE,
							FILE_SHARE_WRITE,
							0,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							0);

	list <int>::size_type cnt = p->frmInfoList.size();
	if( cnt > 0 )
	{
		list<CFrmInfo*>::iterator iter = p->frmInfoList.begin();
		while(iter != p->frmInfoList.end())
		{
			CFrmInfo* pFrmInfo = *iter;
			iter++;
			delete pFrmInfo;
		}
		p->frmInfoList.clear();
	}

	return 1;
}

int CloseSegFile( 
    /* [in] */ TRANS_HANDLE hTr)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	if( p->hLocalFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(p->hLocalFile);
		FILE *fp = fopen(p->filename,"wb");
		if( fp != 0 )
		{
			fclose(fp);
			DeleteFile(p->filename);
		}
	}

	p->hLocalFile = INVALID_HANDLE_VALUE;

	return 1;
}

int SetPictureInfo( 
    /* [in] */ TRANS_HANDLE hTr,
    /* [in] */ __int64 st,
    /* [in] */ __int64 et,
    /* [in] */ int data_len,
    /* [in] */ int bKeyFrame)
{
	//printf("st(%lld), et(%lld), data_len(%d), key(%d)\n",
	//			st,et,data_len,bKeyFrame);

	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	CFrmInfo *pFrmInfo = new CFrmInfo;
	pFrmInfo->st = st;
	pFrmInfo->et = et;
	pFrmInfo->data_len = data_len;
	pFrmInfo->bKeyFrame = bKeyFrame;
	p->frmInfoList.push_back(pFrmInfo);

	return 1;
}

void DoEncode( 
    /* [in] */ TRANS_HANDLE IDL_handle)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(IDL_handle);
	if( findIter == handle_map.end() )
	{
		return;
	}
	else
	{
		p = findIter->second;
	}

	if( p->hLocalFile != INVALID_HANDLE_VALUE )
		CloseHandle(p->hLocalFile);

	p->hLocalFile = INVALID_HANDLE_VALUE;

	_beginthread(ThreadProc,0,p);
}

int OpenCompressedData( 
    /* [in] */ TRANS_HANDLE IDL_handle)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(IDL_handle);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	if( p->CompressedhFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(p->CompressedhFile);
		p->CompressedhFile = INVALID_HANDLE_VALUE;
	}

	p->CompressedhFile = CreateFile(p->compress_filename,
									GENERIC_READ,
									FILE_SHARE_READ,
									0,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0);
	if( p->CompressedhFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	return 1;
}

void CloseCompressedData( 
    /* [in] */ TRANS_HANDLE IDL_handle)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(IDL_handle);
	if( findIter == handle_map.end() )
	{
		return;
	}
	else
	{
		p = findIter->second;
	}

	if( p->CompressedhFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle(p->CompressedhFile);
		FILE *fp = fopen(p->compress_filename,"wb");
		if( fp != 0 )
		{
			fclose(fp);
			DeleteFile(p->compress_filename);
		}
		p->CompressedhFile = INVALID_HANDLE_VALUE;
	}

	list <int>::size_type cnt = p->frmInfoList.size();
	if( cnt > 0 )
	{
		list<CFrmInfo*>::iterator iter = p->frmInfoList.begin();
		while(iter != p->frmInfoList.end())
		{
			CFrmInfo* pFrm = *iter;
			iter++;
			delete pFrm;
		}
		p->frmInfoList.clear();
	}
}

int GetCompressedDataInfo( 
    /* [in] */ TRANS_HANDLE IDL_handle,
    /* [out] */ __int64 *numbers)
{
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(IDL_handle);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	*numbers = 0;
	if( p->CompressedhFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	LARGE_INTEGER li;
	BOOL tf = GetFileSizeEx(p->CompressedhFile, &li);
	*numbers = li.QuadPart;

	li.QuadPart = 0;
	li.LowPart = SetFilePointer(p->CompressedhFile, li.LowPart , &li.HighPart, FILE_BEGIN);

	return 1;
}

int GetCompressedData( 
    /* [in] */ TRANS_HANDLE IDL_handle,
    /* [in] */ int nRead,
    /* [out] */ int *pSize,
    /* [size_is][out][in] */ unsigned char pData[])
{
	DWORD dwRead = 0;

	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(IDL_handle);
	if( findIter == handle_map.end() )
	{
		return 0;
	}
	else
	{
		p = findIter->second;
	}

	*pSize = 0;
	if( p->CompressedhFile == INVALID_HANDLE_VALUE )
	{
		return 0;
	}

	ReadFile(p->CompressedhFile, pData, nRead, &dwRead, 0);

	*pSize = (int)dwRead;

	return 1;
}

///* [async] */ void  AsyncFunc( 
//    /* [in] */ PRPC_ASYNC_STATE AsyncFunc_AsyncHandle,
//    handle_t hBinding,
//    /* [in] */ int nAsychDelay)

/* [async] */ void  AsyncFunc( 
    /* [in] */ PRPC_ASYNC_STATE AsyncFunc_AsyncHandle,
    /* [in] */ TRANS_HANDLE hTr)
{
	RPC_BINDING_HANDLE h = RpcAsyncGetCallHandle(AsyncFunc_AsyncHandle);
	CTransServer *p = 0;
	hash_map<handle_t, CTransServer*>::iterator findIter = handle_map.find(hTr);
	if( findIter == handle_map.end() )
	{
		return;
	}
	else
	{
		p = findIter->second;
	}

	if( p->hLocalFile != INVALID_HANDLE_VALUE )
		CloseHandle(p->hLocalFile);

	p->hLocalFile = INVALID_HANDLE_VALUE;

	ThreadProc(p);

	RPC_STATUS status;
	int nReply = 1;

	status = RpcAsyncCompleteCall(AsyncFunc_AsyncHandle, &nReply);
	printf("RpcAsyncCompleteCall returned 0x%x\n", status);

	// RpcAsyncCompleteCall sample code
	// http://blog.csdn.net/herofyf/article/details/152913
}

// list<CFrmInfo*> frmInfoList;
void __cdecl ThreadProc(void *lp)
{
	CTransServer *pSrv = (CTransServer*)lp;

	CoInitialize(0);

	//GUID	videoType;

	HRESULT hr = 0;
	CESSourceFilter *pESFilter = new CESSourceFilter(pSrv, &hr);
	CNullVideoRenderer *pVR = new CNullVideoRenderer(0,&hr);

	pESFilter->m_hFile = CreateFile(pSrv->filename,
									GENERIC_READ,
									FILE_SHARE_READ,
									0,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0);

	IFilterGraph		*pFG = 0;
	IGraphBuilder		*pBuilder = 0;
	IMediaEvent			*pME = 0;
	IMediaControl		*pMC = 0;

	hr = CoCreateInstance(CLSID_FilterGraph,
									NULL,
									CLSCTX_INPROC,
									IID_IFilterGraph,
									(void**)&pFG);
	hr = pFG->QueryInterface(IID_IGraphBuilder, (void **)&pBuilder);
	hr = pBuilder->AddFilter(pESFilter, L"ES Source Filter");
	hr = pESFilter->AddRef();

	hr = pBuilder->AddFilter(pVR, L"Video Capture Filter");
	hr = pVR->AddRef();

	IBaseFilter *pDecFilter = 0;
	IPin *pPin = 0;
	hr = FindPinByMajorType(pESFilter,
							MEDIATYPE_Video,
							PINDIR_OUTPUT,
							FALSE,
							&pPin);
	if( pPin != 0 )
	{
		CGraphBuilderCB	GraphCB;
		CComQIPtr<IObjectWithSite> pSite(pBuilder);
		if( pSite )
			hr = pSite->SetSite((IUnknown*)&GraphCB);

		hr = AddFilterByCLSID(pBuilder, CLSID_FFDSHOW, &pDecFilter);
		if( pDecFilter != 0 )
		{
			//pESFilter->SetSyncSource(0);
			//pDecFilter->SetSyncSource(0);
			//pVR->SetSyncSource(0);
			hr = ConnectFilters( pBuilder, pPin, pDecFilter );
			hr = ConnectFilters( pBuilder, pDecFilter, pVR);
		}
		else
		{
			hr = pBuilder->Render(pPin);
		}
		pPin->Release();
	}

	if(SUCCEEDED(hr))
	{
		hr = pFG->QueryInterface(IID_IMediaControl, (void **)&pMC);
		if(FAILED(hr))
		{
			goto end_enc;
		}

		hr = pFG->QueryInterface(IID_IMediaEvent, (void **)&pME);
		if(FAILED(hr))
		{
			pMC->Release();
			goto end_enc;
		}

		DWORD dwGraphRegister = 0;
		hr = AddGraphToRot(pBuilder, &dwGraphRegister);

		IMediaFilter *pMediaFilter = 0;
		pBuilder->QueryInterface(IID_IMediaFilter, (void**)&pMediaFilter);
		if( pMediaFilter != 0 )
		{
			hr = pMediaFilter->SetSyncSource(0);
			hr = pMediaFilter->Release();
		}

		LONG levCode;
		OAEVENT oEvent;
		hr = pME->GetEventHandle(&oEvent);
		if(SUCCEEDED(hr))
		{
			hr = pMC->Run();
			if(SUCCEEDED(hr))
			{
				do
				{
					hr = pME->WaitForCompletion(1000, &levCode);
					printf(".");
				}while(hr == E_ABORT);	// time out

				if( EC_COMPLETE == levCode )
					printf("\ndone...EC_COMPLETE \n");
				else if( EC_ERRORABORT == levCode )
					printf("\ndone...EC_ERRORABORT \n");
				else if( EC_USERABORT == levCode )
					printf("\ndone...EC_USERABORT \n");
				else
					printf("\ndone...levCode = %d\n",levCode);
			}
			else
			{
				hr = pMC->Stop();
			}
		}

		//EC_COMPLETE;
		RemoveGraphFromRot(dwGraphRegister);

		pMC->Release();
		pME->Release();
	}

end_enc:

	if( pDecFilter != 0 )
		pDecFilter->Release();

	RemoveFiltersDownstream(pBuilder, pESFilter);
	pVR->Release();

	if( pESFilter->m_hFile != INVALID_HANDLE_VALUE )
	{		
		CloseHandle(pESFilter->m_hFile);
		pESFilter->m_hFile = INVALID_HANDLE_VALUE;
	}

	pESFilter->Release();	
	//delete  pESFilter;

	pFG->Release();
	pBuilder->Release();

	char x264_options[2048];
	char path[MAX_PATH+2];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(0, path, MAX_PATH);
	_splitpath_s( path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );

	//sprintf_s(x264_options, 2048, "%s%sx264-r2377-1ca7bb9.exe --input-depth 8 --input-res %dx%d --intra-refresh -r 2 -q 26 --b-adapt 0 --no-scenecut --keyint 15 -i 15 --bframes 0 -o %s _target_.yuv",
	//				drive, dir, pSrv->m_VideoWidth, pSrv->m_VideoHeight, pSrv->compress_filename);
	sprintf_s(x264_options, 2048, "%s%sx264-r2377-1ca7bb9.exe --input-depth 8 --input-res %dx%d --intra-refresh -r 2 -q 26 --b-adapt 0 --no-scenecut --keyint 15 -i 15 --bframes 0 -o %s %s%s_target_.yuv",
					drive, dir, pSrv->m_VideoWidth, pSrv->m_VideoHeight, pSrv->compress_filename, drive, dir);

	STARTUPINFO StartupInfo = {0,};    //구조체 선언, 초기화
	PROCESS_INFORMATION ProcessInfo;

	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	//Flags 값들이 여러가지가 있습니다.
	//STARTF_USEPOSITION :: dwX, dwY 위치 사용
	//STARTF_USESIZE     :: dwXSize, dwYSize 사용
	//Flags 값이 주어지지 않는 si구조체 값은
	//새로 만들어지는 프로세스에 영향을 주지 않습니다.
	StartupInfo.dwX = 100;
	StartupInfo.dwY = 100;
	StartupInfo.dwXSize = 300;
	StartupInfo.dwYSize = 300;  //dw ~ 는 사실 잘 쓰이지 않습니다.

	StartupInfo.wShowWindow = SW_HIDE;

	StartupInfo.lpTitle = _T(" Child process! ");

	BOOL tf = CreateProcess( NULL,
							x264_options,
							0,	// security attribute
							0,	// thread attribute
							FALSE,	// inherit handle
							CREATE_NEW_CONSOLE | CREATE_NO_WINDOW,	// dwCreationFlags
							0,	// environment
							0,	// current directory
							&StartupInfo,
							&ProcessInfo);
	if( tf == TRUE )
	{
		WaitForSingleObject(ProcessInfo.hProcess, INFINITE);
		CloseHandle( ProcessInfo.hThread);
		CloseHandle( ProcessInfo.hProcess);

		sprintf_s(x264_options, 2048, "%s%s_target_.yuv",drive, dir);
		FILE *fp = fopen(x264_options,"wb");
		if( fp != 0 )
		{
			fclose(fp);
			DeleteFile(x264_options);
			printf("delete yuv\n");
		}

		fp = fopen(pSrv->filename,"wb");
		if( fp != 0 )
		{
			fclose(fp);
			DeleteFile(pSrv->filename);
			printf("delete %s\n",pSrv->filename);
		}
	}

	CoUninitialize();
}
