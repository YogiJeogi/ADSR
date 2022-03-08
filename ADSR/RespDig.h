#pragma once

#define MAX_OUT 16
// CRespDig dialog

class CRespDig : public CDialog
{
	DECLARE_DYNAMIC(CRespDig)

public:
	CRespDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRespDig();

// Dialog Data
	enum { IDD = IDD_DIALOG_Resp };
public:
	static  bool m_bIsRespDigInitialized;
protected:
	void LoadStringVectorInComboBox( CComboBox& comboBox, const CNiStringVector& strings );
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
