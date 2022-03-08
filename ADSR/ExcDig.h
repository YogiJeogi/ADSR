#pragma once
#include "afxwin.h"

// 사용자 정의
#define MAX_INC 12

// CExcDig dialog

class CExcDig : public CDialog
{
	DECLARE_DYNAMIC(CExcDig)

private: 

public:
	CExcDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CExcDig();

// Dialog Data
	enum { IDD = IDD_DIALOG_Exciter };

protected:

	void LoadStringVectorInComboBox(CComboBox& comboBox, 
										const CNiStringVector& strings);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeComboExcHw1();
	afx_msg void OnBnClickedOk();

public:
	static  bool m_bIsExcDigInitialized;
	string		 m_strarryExciterName[MAX_INC];

	virtual BOOL PreTranslateMessage(MSG* pMsg);


};
