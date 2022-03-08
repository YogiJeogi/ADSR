#pragma once


// CStartDAQDlg dialog

class CStartDAQDlg : public CDialog
{
	DECLARE_DYNAMIC(CStartDAQDlg)

public:
	CStartDAQDlg(CWnd* pParent = NULL,bool _IsSI=TRUE);   // standard constructor
	virtual ~CStartDAQDlg();
public:
	using CDialog::Create;
	BOOL Create();

// Dialog Data
	enum { IDD = IDD_DIALOG_DAQ_Start };

protected:
	bool  m_bIsSI;
	CWnd* m_pParent;
	int m_nID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDAQStart();

protected:
	virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedButtonDAQStop();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
