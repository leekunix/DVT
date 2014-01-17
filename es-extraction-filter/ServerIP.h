#pragma once


// CServerIP dialog

class CServerIP : public CDialog
{
	DECLARE_DYNAMIC(CServerIP)

public:
	CServerIP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CServerIP();

// Dialog Data
	enum { IDD = IDD_DIALOG_IP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddIp();
	afx_msg void OnBnClickedButtonDel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
