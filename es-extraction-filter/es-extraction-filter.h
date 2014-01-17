
// es-extraction-filter.h : es-extraction-filter ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#ifdef UNICODE
	#define RPC_STR		unsigned short
#else
	#define RPC_STR		unsigned char
#endif


#define WM_GRAPHEVENT	(WM_APP+100)
#define WM_REMOVEGRAPH	(WM_APP+101)
#define WM_COMPRESSDONE	(WM_APP+102)

class CRPCThread : public CObject
{
public:
	CWnd				*m_pwnd;

	HANDLE				m_hThread;
	bool				m_bThreadExit;

	RPC_STR				*bindingStr;
	handle_t			handleBinding;
	TRANS_HANDLE		m_ctx;
	CString				m_szIP;
	CString				m_nPort;

	RPC_ASYNC_STATE		*m_pAsync;

	__int64				m_fps;

	int					m_nIndex;

	char				es_filename[256];

	CRPCThread();
	~CRPCThread();

	bool	TryRPCConnet(void);
	bool	TryRPCConnet(_In_opt_ RPC_CSTR ProtSeq,    
						_In_opt_ RPC_CSTR NetworkAddr,
						_In_opt_ RPC_CSTR Endpoint);
	void	EncodeStart(void);
	void	OpenRemoteFile(int index);

	void	ReadES(void);
};

class TNalInfo : public CObject
{
public:
	TNalInfo(DWORD bytes)
	{
		m_dwBytes = bytes;
		TCHAR a = 0;
	}

	TNalInfo()
	{
		m_dwBytes = 0;
	}
	DWORD		m_dwBytes;
	LONGLONG	m_llStartTime;
};

class TOvlNalInfo : public TNalInfo
{
public:
	PBYTE m_pNal;

	TOvlNalInfo()
	{
		m_pNal = 0;
	}

	~TOvlNalInfo()
	{
		if( m_pNal != 0 )
			delete [] m_pNal;
	}
};

class CesextractionfilterApp : public CWinApp
{
public:
	CesextractionfilterApp();

	CCriticalSection	m_cs;
	HANDLE				m_sema;
	CObList				m_list;

	DWORD				m_dwCurrentMax;

	CObList				m_rpc_server_list;

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

	void	AddRpcList(CString ip, CString port);
	void	DeleteRpcList(void);

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CesextractionfilterApp theApp;
