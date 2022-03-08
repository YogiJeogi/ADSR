#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CResultVDig dialog

class CResultVDig : public CDialog
{
	DECLARE_DYNAMIC(CResultVDig)

public:
	static BOOL m_bIsWhiteNoiseTimeView;
	static BOOL m_bIsDesiredSignalTimeView;
public:
	using CDialog::Create;
protected:
	CResultVDig(CWnd* pParent = NULL);   // standard constructor
public:
	CResultVDig(int Type,CWnd* pParent = NULL);   // standard constructor
	virtual ~CResultVDig();

	BOOL Create();

// Dialog Data
	enum { IDD = IDD_DIALOG_ResultV };
// ¸É¹ö¼±¾ð
public:
	CDocument * m_pCurrentDoc;

protected:
	CWnd* m_pParent;
	int m_nID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:

protected:
	virtual void OnCancel();
private:
	CStatic m_CtrlStaticSS;
	CStatic m_CtrlStaticVM;
	CStatic m_CtrlStaticH;
	CStatic m_CtrlStaticSSComp;

	CButton m_CtrlButtonHor;
	CButton m_CtrlButtonCas;
	CButton m_CtrlButtonAllClose;
	CButton m_CtrlButtonClose;
	CButton m_CtrlButtonView;

	CButton m_CtrlCheckTime;
	CButton m_CtrlCheckPSD;
	CButton m_CtrlCheckXY;
	CButton m_CtrlCheckStatistics;

	CListCtrl m_CtrlListCh;
	CListCtrl m_CtrlListCh2;

	unsigned int TYPE;

public:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedRadio0();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedButtonWinHor();
	afx_msg void OnBnClickedButtonWinCas();
	afx_msg void OnBnClickedButtonWinAllclose();
	afx_msg void OnBnClickedView();

};
