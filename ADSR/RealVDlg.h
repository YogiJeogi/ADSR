#pragma once
#include "afxwin.h"


// CRealVDlg dialog

class CRealVDlg : public CDialog
{
	DECLARE_DYNAMIC(CRealVDlg)

public:
	CRealVDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRealVDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_GRAPH_Panel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:


	int m_nTypeGraph;

	CListBox m_CtrlEList;
	CListBox m_CtrlRList;
	CButton m_CtrlExcCheck;
	CButton m_CtrlResCheck;

	CArray<int, int> m_aryEListSel;
	CArray<int, int> m_aryRListSel;

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckResall();
	afx_msg void OnBnClickedRadioRespTh();
	afx_msg void OnBnClickedRadioSiFrf();
	afx_msg void OnBnClickedRadioTsPsd();
	afx_msg void OnBnClickedCheckExcall();
	afx_msg void OnBnClickedCheckRes();
	afx_msg void OnBnClickedCheckExc();
	afx_msg void OnBnClickedButtonGraphCreate();

};
