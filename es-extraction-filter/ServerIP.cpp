// ServerIP.cpp : implementation file
//

#include "stdafx.h"
#include "es-extraction-filter.h"
#include "ServerIP.h"
#include "afxdialogex.h"


// CServerIP dialog

IMPLEMENT_DYNAMIC(CServerIP, CDialog)

CServerIP::CServerIP(CWnd* pParent /*=NULL*/)
	: CDialog(CServerIP::IDD, pParent)
{

}

CServerIP::~CServerIP()
{
}

void CServerIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerIP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_IP, &CServerIP::OnBnClickedButtonAddIp)
	ON_BN_CLICKED(IDC_BUTTON_Del, &CServerIP::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CServerIP message handlers

void CServerIP::OnBnClickedButtonAddIp()
{
	INT_PTR list_size = theApp.m_rpc_server_list.GetCount();
	CListBox *pbox = (CListBox*)GetDlgItem(IDC_LIST_IP);
	CString str;

	GetDlgItem(IDC_EDIT_IP)->GetWindowText(str);
	pbox->AddString(str);

	str=_T("");
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(str);
}

void CServerIP::OnBnClickedButtonDel()
{
	CListBox *pbox = (CListBox*)GetDlgItem(IDC_LIST_IP);
	int idx = pbox->GetCurSel();
	if( idx == LB_ERR )
		return;

	pbox->DeleteString(idx);
}

BOOL CServerIP::OnInitDialog()
{
	CDialog::OnInitDialog();

	POSITION pos = 0;
	CListBox *pbox = (CListBox*)GetDlgItem(IDC_LIST_IP);

	CString str;
	pos = theApp.m_rpc_server_list.GetHeadPosition();
	while( pos != NULL )
	{
		CRPCThread *p = (CRPCThread*)theApp.m_rpc_server_list.GetNext(pos);		
		str.Format(_T("%s"),p->m_szIP);
		pbox->AddString(str);
	}

	str=_T("127.0.0.1");
	GetDlgItem(IDC_EDIT_IP)->SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CServerIP::OnOK()
{
	CListBox *pbox = (CListBox*)GetDlgItem(IDC_LIST_IP);
	int count = pbox->GetCount();

	theApp.DeleteRpcList();

	for( int i = 0 ; i < count ; i++ )
	{
		CRPCThread *p = new CRPCThread;
		p->m_nPort.Format(_T("%s"),DEFAULT_ENDPOINT);
		pbox->GetText(i, p->m_szIP);
		theApp.m_rpc_server_list.AddTail(p);
	}

	//

	CDialog::OnOK();
}
