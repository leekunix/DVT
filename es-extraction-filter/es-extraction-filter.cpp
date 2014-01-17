
// es-extraction-filter.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "es-extraction-filter.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CesextractionfilterApp

BEGIN_MESSAGE_MAP(CesextractionfilterApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CesextractionfilterApp::OnAppAbout)
END_MESSAGE_MAP()


// CesextractionfilterApp ����

CesextractionfilterApp::CesextractionfilterApp()
{
	CoInitialize(0);
	m_sema = 0;
	m_dwCurrentMax = 0;
}

// ������ CesextractionfilterApp ��ü�Դϴ�.

CesextractionfilterApp theApp;


// CesextractionfilterApp �ʱ�ȭ

BOOL CesextractionfilterApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	//if (!AfxOleInit())
	//{
	//	AfxMessageBox(IDP_OLE_INIT_FAILED);
	//	return FALSE;
	//}
	//AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	//SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));

	m_sema = CreateSemaphore(0,0,100,0);

	// �� â�� ����� ���� �� �ڵ忡���� �� ������ â ��ü��
	// ���� ���� �̸� ���� ���α׷��� �� â ��ü�� �����մϴ�.
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;

	m_pMainWnd = pFrame;

	// �������� ����� ���ҽ��� �Բ� �ε��մϴ�.
	BOOL tf = pFrame->LoadFrame(IDR_MAINFRAME,
								WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
								NULL);
	if( tf == FALSE )
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	return TRUE;
}

// CesextractionfilterApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CesextractionfilterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CesextractionfilterApp �޽��� ó����

void CesextractionfilterApp::AddRpcList(CString ip, CString port)
{
}

void CesextractionfilterApp::DeleteRpcList(void)
{
	POSITION pos;

	INT_PTR size = m_rpc_server_list.GetCount();
	if( size > 0 )
	{
		for( pos = m_rpc_server_list.GetHeadPosition(); pos != NULL; )
		{
			CRPCThread *p = (CRPCThread*)m_rpc_server_list.GetNext(pos);
			delete p;
		}
		m_rpc_server_list.RemoveAll();
	}
}

int CesextractionfilterApp::ExitInstance()
{
	if( m_sema != 0 )
		CloseHandle(m_sema);

	POSITION pos;
	for( pos = m_list.GetHeadPosition(); pos != NULL; )
	{
		TNalInfo *p = (TNalInfo*)m_list.GetNext(pos);
		delete p;
	}
	m_list.RemoveAll();

	DeleteRpcList();

	CoUninitialize();

	return CWinApp::ExitInstance();
}
