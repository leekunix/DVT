
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "es-extraction-filter.h"

#include "MainFrm.h"
#include "ServerIP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define		HIST_BIN_WIDTH		(9)
#define		SCROLLBAR_HEIGHT	(20)
#define		HIST_START_X		(10)
#define		HIST_BG_COLOR		RGB(0xeb,0xed,0xec)
#define		HIST_BORDER_COLOR	RGB(0x85,0x85,0x85)


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	//ON_COMMAND(IDM_PLAYBACK, &CMainFrame::OnPlayback)
	ON_MESSAGE(WM_GRAPHEVENT, OnGraph)
	ON_MESSAGE(WM_REMOVEGRAPH, OnRemoveGraph)
	ON_MESSAGE(WM_COMPRESSDONE, OnCompressDone)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_IP_REG, &CMainFrame::OnIpReg)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸
CMainFrame::CMainFrame()
{
	m_pFG = 0;
	m_pBuilder = 0;
	m_pSrcFilter = 0;	
	m_pMediaEvent = 0;
	m_pMC = 0;
	m_pMediaPosition = 0;
	m_pExtFilter = 0;
	m_dwGraphRegister = 0;
	m_nFrameCnt = m_nShownPos = 0;
	m_nHistDrawnCnt = 0;
	m_nCurrentFrmIndex = 0;
	m_nScrollPos = 0;
	m_nShownPos = -1;
}

CMainFrame::~CMainFrame()
{
	ReleaseFilters();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	DragAcceptFiles(TRUE);

	CRect	client_rect,r;
	GetClientRect(&client_rect);

	r = client_rect;
	r.left = HIST_START_X;
	r.right -= HIST_START_X;
	r.top = 0;
	r.bottom = r.top + 30;
	m_wndByteEdit.Create( WS_CHILD | WS_CLIPSIBLINGS | ES_READONLY | WS_VISIBLE | ES_CENTER | ES_AUTOHSCROLL,
								r, this, 0);
	m_wndByteEdit.ShowWindow(SW_HIDE);
	//m_wndByteEdit.SetWindowText(_T("On loading..."));

	r = client_rect;
	r.top = client_rect.bottom - SCROLLBAR_HEIGHT + 2;
	r.left = HIST_START_X;
	r.right -= HIST_START_X;
	r.bottom -= 2;
	m_scbar.Create( SBS_HORZ | WS_CHILD | WS_CLIPSIBLINGS | WS_VISIBLE, r, this, 0 );
	m_scbar.ShowWindow(SW_HIDE);
	m_scbar.EnableScrollBar(ESB_ENABLE_BOTH);	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_MINIMIZEBOX | WS_SYSMENU;
	//cs.cx = 240;
	//cs.cy = 160;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;

	LPCTSTR strMyClass = AfxRegisterWndClass(
									CS_VREDRAW | CS_HREDRAW,// );
									::LoadCursor(NULL, IDC_ARROW),
									(HBRUSH) ::GetStockObject(LTGRAY_BRUSH),
									::LoadIcon(NULL, IDI_APPLICATION));

	cs.lpszClass = strMyClass;//AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR *lpszPath = 0;
	UINT i = 0;
	UINT bufferSize = 0;
	UINT uiFileNum = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	if( uiFileNum <= 0 )
		return;

	INT_PTR cnt = theApp.m_rpc_server_list.GetCount();
	if( cnt <= 0 )
	{
		AfxMessageBox(_T("there is no server"));
		return;
	}


	m_nFrameCnt = 0;
	Invalidate();

	if( m_pFG != 0 )
	{		
		ReleaseFilters();

		POSITION pos;
		for( pos = theApp.m_list.GetHeadPosition(); pos != NULL; )
		{
			TNalInfo *p = (TNalInfo*)theApp.m_list.GetNext(pos);
			delete p;
		}
		theApp.m_list.RemoveAll();		
	}

	//for ( UINT i = 0 ; i < uiFileNum ; i++ )
	{
		bufferSize = DragQueryFile(hDropInfo, i, 0, 0) + 1;
		lpszPath = new TCHAR[bufferSize];

		DragQueryFile(hDropInfo, i, (LPTSTR)lpszPath, bufferSize);

		HRESULT hr = CoCreateInstance(CLSID_FilterGraph,
										NULL,
										CLSCTX_INPROC,
										IID_IFilterGraph,
										(void**)&m_pFG);
		hr = m_pFG->QueryInterface(IID_IGraphBuilder, (void **)&m_pBuilder);

		USES_CONVERSION;
		hr = m_pBuilder->AddSourceFilter( T2W(lpszPath),
										0,
										&m_pSrcFilter);
		if( FAILED(hr) )
		{
			delete [] lpszPath;
			AfxMessageBox(_T("Can't playback the file"));
			return;
		}

		m_pSrcFilter->SetSyncSource(0);

		IPin *pPin = 0;
		IEnumPins *pEnum = 0;
		PIN_DIRECTION pinDir;
		int nFound = 0;

		hr = m_pSrcFilter->EnumPins(&pEnum);
		while (S_OK == pEnum->Next(1, &pPin, 0))
		{
			hr = pPin->QueryDirection(&pinDir);
			pPin->Release();
			if( SUCCEEDED(hr) && pinDir == PINDIR_OUTPUT )
			{
				nFound++;
			}
		}
		pEnum->Release();

		if( nFound == 1 )
		{
			// mediatype_stream.. maybe..
			hr = FindPinByMajorType(m_pSrcFilter,
									MEDIATYPE_Stream,
									PINDIR_OUTPUT,
									FALSE,
									&pPin);
			if( SUCCEEDED(hr) )
			{
				m_pExtFilter = new CVideoExtract(0,&hr);
				hr = m_pBuilder->AddFilter(m_pExtFilter, L"GC. VideoExtract");
				ASSERT(hr==S_OK);
				m_pExtFilter->AddRef();

				hr = ConnectFilters(m_pBuilder,pPin,m_pExtFilter);
				pPin->Release();
				if( FAILED(hr) )
				{
					SAFE_RELEASE(m_pSrcFilter)
					SAFE_RELEASE(m_pMediaPosition)
					SAFE_RELEASE(m_pMediaEvent)
					SAFE_RELEASE(m_pMC)
					SAFE_RELEASE(m_pBuilder)
					SAFE_RELEASE(m_pFG)

					delete m_pExtFilter;
					m_pExtFilter = 0;

					delete [] lpszPath;

					AfxMessageBox(_T("can't connect filters"));
					return;
				}

				m_pExtFilter->SetSyncSource(0);
			}
		} // if( nFound == 1 )
		else if( nFound >= 2 )
		{
			hr = FindPinByMajorType(m_pSrcFilter,
									MEDIATYPE_Video,
									PINDIR_OUTPUT,
									FALSE,
									&pPin);
			if( SUCCEEDED(hr) )
			{
				m_pExtFilter = new CVideoExtract(0,&hr);
				hr = m_pBuilder->AddFilter(m_pExtFilter, L"GC. VideoExtract");
				ASSERT(hr==S_OK);
				m_pExtFilter->AddRef();

				hr = ConnectFilters(m_pBuilder,pPin,m_pExtFilter);
				pPin->Release();
				if( FAILED(hr) )
				{
					SAFE_RELEASE(m_pSrcFilter)
					SAFE_RELEASE(m_pMediaPosition)
					SAFE_RELEASE(m_pMediaEvent)
					SAFE_RELEASE(m_pMC)
					SAFE_RELEASE(m_pBuilder)
					SAFE_RELEASE(m_pFG)

					delete m_pExtFilter;
					m_pExtFilter = 0;

					delete [] lpszPath;

					AfxMessageBox(_T("can't connect filters"));
					return;
				}
				m_pExtFilter->SetSyncSource(0);
			}
		}

		m_pBuilder->QueryInterface(IID_IMediaPosition, (void**)&m_pMediaPosition);
		if( m_pMediaPosition != 0 )
		{
			REFTIME length = 0;
			hr = m_pMediaPosition->get_Duration(&length);
			if( SUCCEEDED(hr) )
			{
				m_pExtFilter->m_duration = (REFERENCE_TIME)(length*1000);		// ms unit
				m_pExtFilter->avg_segment_length = 
								(REFERENCE_TIME)(length/m_pExtFilter->m_partitions);
			}
		}		

		hr = AddGraphToRot(m_pBuilder, &m_dwGraphRegister);

		//m_pExtFilter->SetFileName(_T("E:/Downloads/_aa.264"));

		////m_pBuilder->SetDefaultSyncSource();
		IMediaFilter *pMediaFilter = 0;
		m_pBuilder->QueryInterface(IID_IMediaFilter, (void**)&pMediaFilter);
		if( pMediaFilter != 0 )
		{
			hr = pMediaFilter->SetSyncSource(0);
			hr = pMediaFilter->Release();
		}
	}

	delete [] lpszPath;

	//CFrameWnd::OnDropFiles(hDropInfo);
	DragFinish(hDropInfo);

	HRESULT hr = 0;
	hr = m_pBuilder->QueryInterface(IID_IMediaControl,(LPVOID*) &m_pMC);
	hr = m_pBuilder->QueryInterface(IID_IMediaEventEx,(LPVOID*) &m_pMediaEvent);
	if( m_pMediaEvent != 0 )
	{
		hr = m_pMediaEvent->SetNotifyWindow( (OAHWND)GetSafeHwnd(), WM_GRAPHEVENT, 0 );
		hr = m_pMediaEvent->SetNotifyFlags(0);
	}
	hr = m_pMC->Run();

	m_nDoneCount = 0;

	m_wndByteEdit.ShowWindow(SW_SHOW);
	m_wndByteEdit.SetWindowText(_T("On loading..."));
}

void CMainFrame::OnPlayback()
{
	HRESULT hr = 0;
	if( m_pFG != 0 && m_pExtFilter != 0 )
	{
		hr = m_pBuilder->QueryInterface(IID_IMediaControl,(LPVOID*) &m_pMC);
		hr = m_pBuilder->QueryInterface(IID_IMediaEventEx,(LPVOID*) &m_pMediaEvent);
		if( m_pMediaEvent != 0 )
		{
			hr = m_pMediaEvent->SetNotifyWindow( (OAHWND)GetSafeHwnd(), WM_GRAPHEVENT, 0 );
			hr = m_pMediaEvent->SetNotifyFlags(0);
		}

		hr = m_pMC->Run();
	}
}

void CMainFrame::ReleaseFilters(void)
{
	if( m_dwGraphRegister != 0 )
	{
		RemoveGraphFromRot(m_dwGraphRegister);
		m_dwGraphRegister = 0;
	}

	if( m_pFG != 0 )
	{
		if( m_pMC != 0 )
			m_pMC->Stop();

		if( m_pMediaEvent != 0 )
			m_pMediaEvent->SetNotifyWindow(NULL, 0, 0);

		RemoveDownstream(m_pBuilder, m_pSrcFilter);

		if( m_pExtFilter != 0 )
		{
			delete m_pExtFilter;
			m_pExtFilter = 0;
		}

		SAFE_RELEASE(m_pSrcFilter)
		SAFE_RELEASE(m_pMC)
		SAFE_RELEASE(m_pMediaPosition)
		SAFE_RELEASE(m_pMediaEvent)
		SAFE_RELEASE(m_pBuilder)
		SAFE_RELEASE(m_pFG)
	}
}

LRESULT CMainFrame::OnRemoveGraph(WPARAM wp, LPARAM lp)
{
	m_wndByteEdit.SetWindowText(_T("Complete..."));
	ReleaseFilters();
	return 0;
}

LRESULT CMainFrame::OnCompressDone(WPARAM wp, LPARAM lp)
{
	CRPCThread *p = (CRPCThread*)lp;

	m_nDoneCount++;

	char path[MAX_PATH+2];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	GetModuleFileName(0, path, MAX_PATH);
	_splitpath_s( path, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT );	

	INT_PTR cnt = theApp.m_rpc_server_list.GetCount();
	if( cnt == m_nDoneCount )
	{
		//FILE *hMergeFile = fopen("_result.es","wb");
		sprintf_s(path,MAX_PATH,"%s%s_result.es",drive,dir);
		FILE *hMergeFile = fopen(path,"wb");
		int DATA_MAX = 1024*1024*5;
		unsigned char *pBuff0 = new unsigned char[DATA_MAX+64];
		unsigned char *pBuff = pBuff0 + 64;

		for( INT_PTR i = 0 ; i < cnt ; i++ )
		{
			char fname[256];
			sprintf_s(fname,255,"%s%s%d.es",drive, dir, i);
			FILE *esFile = fopen(fname,"rb");

			assert( esFile != 0 );

			size_t sizeRead = 0;
			do
			{
				 sizeRead = fread( pBuff, 1, DATA_MAX, esFile );
				 if( sizeRead > 0 )
				 {
					 fwrite( pBuff, sizeRead, 1, hMergeFile );
				 }

			}while(sizeRead > 0);

#if 0
			int i_data = 0, b_eof = 0;
			do
			{
				unsigned char *p, *p_next, *end;
				int i_read = 0, i_size = 0;

				if( i_data < DATA_MAX && !b_eof )
				{
					i_read = fread( (pBuff + i_data), 1, DATA_MAX - i_data, esFile );
					if( i_read < 0 )
						b_eof = 1;
					else
						i_data += i_read;
				}

				if( i_data < 3 )
					break;

				end = pBuff + i_data;
				p = pBuff;

				// first
				int zero_cnt = 0;
				while(p < (end-3) )
				{
					if( p[0] == 0x00 && p[1] == 0x00 && p[2] == 0x01 )
					{
						break;
					}
					zero_cnt++;
					p++;
				}

				if( p >= end - 3 )
				{
					//fprintf( stderr, "garbage (i_data = %d)\n", i_data );
					i_data = 0;
					continue;
				}

				// Search end of NAL
				p_next = p + 3;
				while( p_next < end - 3 )
				{
					if( p_next[0] == 0x00 && p_next[1] == 0x00 && p_next[2] == 0x01 )
					{
						zero_cnt++;
						break;
					}
					p_next++;
				}

				if( p_next == end - 3 && i_data < DATA_MAX )
					p_next = end;
				
				i_size = p_next - p - zero_cnt;
				if( i_size <= 0 )
				{
					if( b_eof )
						break;

					//fprintf( stderr, "nal too large (FIXME) ?\n" );
					i_data = 0;
					continue;
				}
				else if( i_size > 1 && p[i_size] == 0 )
				{
					i_size--;
				}

				if( (p[3] & 0x1f) == 6 && p[4] == 5 )
				{
					// skip sei user data
				}
				else
				{
					fwrite( p, i_size, 1, hMergeFile );
				}

				memmove( pBuff, p_next, end - p_next );
				i_data -= p_next - pBuff;
			}while(1);
#endif

			fclose(esFile);

			esFile = fopen(fname,"wb");
			fclose(esFile);
			DeleteFile(fname);
		}

		delete [] pBuff0;
		fclose(hMergeFile);

		float fps = 10000000.0/p->m_fps;

		char avi_options[2048];
		sprintf_s(avi_options, 2048, "%s%savc2avi.exe -f %.2f -i %s%s_result.es -o %s%s_result.avi",
							drive, dir, fps, drive, dir, drive, dir);

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
								avi_options,
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

			AfxMessageBox(_T("Done!"));
		}
	}
	//theApp.m_rpc_server_list.FindIndex(m_idx);

	return 0;
}

LRESULT CMainFrame::OnGraph(WPARAM wp, LPARAM lp)
{
	long EventCode, Param1, Param2;
	while (m_pMediaEvent->GetEvent(&EventCode, &Param1, &Param2, 0) != E_ABORT)
	{
		TRACE("CMainFrame::OnGraph : EventCode : 0x%x\n",EventCode);
		switch (EventCode)
		{
		case EC_COMPLETE:
			{
				m_pMC->Stop();
				//m_pMediaPosition->put_CurrentPosition(0);
				TRACE("end of graph\n");

				theApp.m_cs.Lock();
				m_nFrameCnt = theApp.m_list.GetCount();

				POSITION pos;
				for( pos = theApp.m_list.GetHeadPosition(); pos != NULL; )
				{
					TNalInfo *p = (TNalInfo*)theApp.m_list.GetNext(pos);
					if( p->m_dwBytes > theApp.m_dwCurrentMax )
						theApp.m_dwCurrentMax = p->m_dwBytes;
				}
				theApp.m_cs.Unlock();

				CRect	r;
				GetClientRect(&r);

				int	hist_w = r.Width();

				hist_w = (hist_w/(1+HIST_BIN_WIDTH)) * (1+HIST_BIN_WIDTH);
				m_nHistDrawnCnt = hist_w/(1+HIST_BIN_WIDTH);

				if( m_nHistDrawnCnt < m_nFrameCnt )
				{
					m_scbar.ShowWindow(SW_SHOW);
					m_scbar.SetScrollRange(0, m_nFrameCnt - m_nHistDrawnCnt);
				}
				else
				{
					m_scbar.ShowWindow(SW_HIDE);
					m_scbar.SetScrollRange(0, m_nFrameCnt);
				}
				m_scbar.SetScrollPos(0);

				//m_wndByteEdit.ShowWindow(SW_HIDE);

				Invalidate();

				PostMessage(WM_REMOVEGRAPH);
			}
			// here when media is completely done playing
			//if (!g_Looping)
			//	g_pMediaControl->Stop();
			//g_pMediaPosition->put_CurrentPosition(0);   // reset to beginning
			break;

		default:
			break;
		}	
		m_pMediaEvent->FreeEventParams(EventCode, Param1, Param2);
	}
	return 0;
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	CRect	rect,r;
	GetClientRect(&r);

	rect = r;
	rect.top = rect.bottom - SCROLLBAR_HEIGHT + 2;
	rect.left = HIST_START_X;
	rect.right -= HIST_START_X;
	rect.bottom -= 2;
	if( IsWindow(m_scbar.GetSafeHwnd()) )
	{
		m_scbar.MoveWindow(rect);
	}

	rect = r;
	rect.top = 0;
	rect.left = HIST_START_X;
	rect.right -= HIST_START_X;
	rect.bottom = r.top + 30;
	if( IsWindow(m_wndByteEdit.GetSafeHwnd()) )
	{
		m_wndByteEdit.MoveWindow(rect);
	}

	////////////////////////////////////////////////////////////
	rect = r;
	r.top = rect.Height()/2;
	r.left = HIST_START_X;
	r.right -= HIST_START_X;
	r.bottom -= 2;
	r.top++;
	r.left++;
	r.bottom--;
	r.right--;
	int	hist_w = r.Width();
	hist_w = (hist_w/(1+HIST_BIN_WIDTH)) * (1+HIST_BIN_WIDTH);
	m_nHistDrawnCnt = hist_w/(1+HIST_BIN_WIDTH);

	if( m_nFrameCnt > 0 )
	{
		if( m_nHistDrawnCnt < m_nFrameCnt )
		{
			m_scbar.ShowWindow(SW_SHOW);
			m_scbar.SetScrollRange(0, m_nFrameCnt - m_nHistDrawnCnt);
		}
		else
		{
			m_scbar.ShowWindow(SW_HIDE);
			m_scbar.SetScrollRange(0, m_nFrameCnt);
		}
		m_scbar.SetScrollPos(0);
	}
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::DrawHistogram(CDC *pdc)
{
	CRect	rect, r, bin_r;
	INT_PTR i = 0;

	if( m_nFrameCnt == 0 )
		return;

	GetClientRect(&r);
	rect = r;

	r.top = rect.Height()/2 - rect.Height()/4;
	r.left = HIST_START_X;
	r.right -= HIST_START_X;
	r.bottom -= SCROLLBAR_HEIGHT+2;
	CBrush	br( RGB(10,10,10) );
	pdc->Draw3dRect(r, HIST_BORDER_COLOR, HIST_BORDER_COLOR);

	// border
	r.top = r.top + 22;
	r.left = r.left + 2;
	r.bottom = r.bottom - 2;
	r.right = r.right - 6;
	//pdc->FillSolidRect(&r,HIST_BG_COLOR);
	pdc->FillSolidRect(&r,RGB(1,1,1));

	theApp.m_cs.Lock();
	INT_PTR list_cnt = theApp.m_list.GetCount();
	theApp.m_cs.Unlock();
	if( list_cnt <= 0 )
	{
		return;
	}

	m_HistRect = r;

	int	nPos = m_nScrollPos;
	int	hist_w = r.Width();
	if( hist_w < 3 )
	{
		return;
	}

	hist_w = (hist_w/(1+HIST_BIN_WIDTH)) * (1+HIST_BIN_WIDTH);
	int hist_h = r.Height()-4;
	m_nHistDrawnCnt = hist_w/(1+HIST_BIN_WIDTH);
	int cnt = m_nHistDrawnCnt + nPos;
	
	POSITION	pos;

#define		I_SLICE_BIN_COLOR	RGB(0x55,0x55,0x55)
#define		P_SLICE_BIN_COLOR	RGB(0x88,0x89,0x89)
	COLORREF	clr[2] = { P_SLICE_BIN_COLOR , I_SLICE_BIN_COLOR };
	
	bin_r.bottom = r.bottom - 1;

	if( nPos >= (m_nFrameCnt-m_nHistDrawnCnt) )
		nPos = (m_nFrameCnt-m_nHistDrawnCnt);

	theApp.m_cs.Lock();
	for( i = 0 ; i < m_nHistDrawnCnt ; i++ )
	{
		TNalInfo *p = 0;
		pos = theApp.m_list.FindIndex(i+nPos);
		if( pos == 0 )
			break;

		try
		{
			p = (TNalInfo*)theApp.m_list.GetAt(pos);
		}
		catch(...)
		{
			return;
		}
		bin_r.top = bin_r.bottom - ( ( hist_h * p->m_dwBytes )/theApp.m_dwCurrentMax );
		bin_r.left = r.left + i * (HIST_BIN_WIDTH+1);
		bin_r.right = HIST_BIN_WIDTH + bin_r.left;

		if( bin_r.top == bin_r.bottom )
		{
			bin_r.top -= 1;
		}

		//if( p->m_FrameType == I_SLICE || p->m_FrameType == I_ALL_SLICE )
		//{
		//	pdc->FillSolidRect(&bin_r,clr[1]);
		//}
		//else
		{
			pdc->FillSolidRect(&bin_r,clr[0]);
		}
	}

	m_nShownPos = nPos;

	if( m_nCurrentFrmIndex != -1 )
	{
		i = m_nCurrentFrmIndex + nPos;
		if( i >= m_nFrameCnt )
		{
			theApp.m_cs.Unlock();
			return;
		}

		pos = theApp.m_list.FindIndex(i);
		TNalInfo *p = (TNalInfo*)theApp.m_list.GetAt(pos);

		bin_r.top = bin_r.bottom - hist_h;
		bin_r.left = r.left + m_nCurrentFrmIndex * (1+HIST_BIN_WIDTH) + HIST_BIN_WIDTH/2;
		bin_r.right = bin_r.left + 1;
		pdc->FillSolidRect(&bin_r,RGB(255,0,0));

		CString	str;
		str.Format(_T("[%d] : %lld(ms), %d bytes"),i,p->m_llStartTime/10000,p->m_dwBytes);
		m_wndByteEdit.SetWindowText(str);
	}
	theApp.m_cs.Unlock();
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFrameWnd::OnPaint()을(를) 호출하지 마십시오.

	DrawHistogram(&dc);
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( m_scbar.GetSafeHwnd() == pScrollBar->GetSafeHwnd() )
	{
		switch( nSBCode )
		{
		case SB_LEFT:	// 6
			m_nScrollPos = nPos = 0;
			m_nCurrentFrmIndex = 0;
			TRACE("OnHScroll : SB_LEFT\r\n");
			break;

		case SB_ENDSCROLL:
			TRACE("OnHScroll : SB_ENDSCROLL,, %d, m_nCurrentFrmIndex(%d)\r\n",m_nScrollPos,m_nCurrentFrmIndex);
			{
				m_scbar.SetScrollPos(m_nScrollPos);
				//m_pParent->PostMessage(WM_GOTO_FRAME,1,m_nCurrentFrmIndex+m_nScrollPos);
			}
			break;

		case SB_LINELEFT:	//  화살표 누른경우
			if( m_nScrollPos > m_nHistDrawnCnt && m_nCurrentFrmIndex == 0 )
			{
				nPos = m_nScrollPos-1;
			}
			else
			{
				if( m_nScrollPos != 0 && m_nCurrentFrmIndex == 0 )
					m_nScrollPos--;

				if( m_nCurrentFrmIndex > 0 )
					m_nCurrentFrmIndex--;
				
				nPos = m_nScrollPos;
			}
			
			TRACE("OnHScroll : SB_LINELEFT\r\n");
			break;

		case SB_LINERIGHT:	//  화살표 누른경우
			if( (1+m_nScrollPos+m_nHistDrawnCnt) < m_nFrameCnt )
			{
				nPos = m_nScrollPos+1;
			}
			else
			{
				//nPos = m_nScrollPos;
				nPos = m_nFrameCnt - m_nHistDrawnCnt;

				if( (1+m_nCurrentFrmIndex) < m_nHistDrawnCnt )
					m_nCurrentFrmIndex++;
			}

			TRACE("OnHScroll : SB_LINERIGHT\r\n");
			break;

		case SB_PAGELEFT:
			if( m_nScrollPos >= m_nHistDrawnCnt )
			{
				nPos = m_nScrollPos-m_nHistDrawnCnt;
			}
			else
			{
				nPos = 0;
				m_nCurrentFrmIndex = 0;
			}
			TRACE("OnHScroll : SB_PAGELEFT\r\n");
			break;

		case SB_PAGERIGHT:
			if( (2*m_nHistDrawnCnt+m_nScrollPos) < m_nFrameCnt )
			{
				nPos = m_nScrollPos+m_nHistDrawnCnt;
			}
			else
			{
				nPos = m_nFrameCnt - m_nHistDrawnCnt;
				m_nCurrentFrmIndex = 0;
			}
			TRACE("OnHScroll : SB_PAGERIGHT\r\n");
			break;

		case SB_RIGHT:	// 1
			m_nScrollPos = nPos = m_nFrameCnt;
			m_nCurrentFrmIndex = m_nHistDrawnCnt;
			TRACE("OnHScroll : SB_RIGHT\r\n");
			break;

		case SB_THUMBPOSITION:
			TRACE("OnHScroll : SB_THUMBPOSITION (%d)\r\n",nPos);
			if( nPos == m_nFrameCnt-1 )
			{
				m_nCurrentFrmIndex = m_nHistDrawnCnt;
			}
			break;

		case SB_THUMBTRACK:
			TRACE("OnHScroll : SB_THUMBTRACK nPos(%d)\r\n",nPos);
			if( nPos == m_nFrameCnt-1 )
			{
				m_nCurrentFrmIndex = m_nHistDrawnCnt;
			}
			break;
		}

		if( SB_ENDSCROLL != nSBCode )
		{
			m_nScrollPos = nPos;
		}

		CClientDC	dc(this);
		DrawHistogram(&dc);
	}

	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CRect r;
	//GetClientRect(&r);
	//if( nFlags == MK_LBUTTON && point.y <= m_bitmapInfo.bmHeight && point.x <= (r.right-10) )
	//{
	//	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKEWPARAM(point.x, point.y));
	//	return;
	//}

	if( (nFlags == MK_LBUTTON) && 
		(m_nFrameCnt > 0) && 
		(m_HistRect.PtInRect(point) == TRUE) )
	{
		point.x -= HIST_START_X;
		int idx = point.x / (1+HIST_BIN_WIDTH);
		if( m_nCurrentFrmIndex != idx )
		{
			m_nCurrentFrmIndex = idx;
			CClientDC	dc(this);
			DrawHistogram(&dc);

			//m_pParent->PostMessage(WM_GOTO_FRAME,1,m_nScrollPos+idx);
		}
	}

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CMainFrame::OnIpReg()
{
	CServerIP	IPDlg;
	if( IDOK==IPDlg.DoModal() )
	{
		POSITION pos = 0;
		CListBox *pbox = (CListBox*)GetDlgItem(IDC_LIST_IP);

		pos = theApp.m_rpc_server_list.GetHeadPosition();
		while( pos != NULL )
		{
			CRPCThread *p = (CRPCThread*)theApp.m_rpc_server_list.GetNext(pos);
			p->m_pwnd = this;
			p->TryRPCConnet();
		}
	}
}
