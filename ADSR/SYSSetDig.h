#pragma once
#include "afxwin.h"
#define MAX_OUT 16


// CSYSSetDig dialog

class CSYSSetDig : public CDialog
{
	DECLARE_DYNAMIC(CSYSSetDig)

public:
	CSYSSetDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSYSSetDig();

// Dialog Data
	enum { IDD = IDD_DIALOG_InOut };

public:
// ����� ���� �Լ�
	void HowManyHWExist();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:

public:

	// ���� �ɹ� ����
	static bool m_bIsInitialized;

	// �ɹ� ����
/*	CString m_strFS;*/
	int m_nInput;
	int m_nResp;
	int m_nSubResp;
// 	CString m_strStopFre;
// 	CString m_strStartFre;
	int m_nBlockSize;
	int m_nCyclicAvg;
	int m_nWindow;
	int m_nOverlap;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReconnectHw();
	afx_msg void OnEnKillfocusEditnslavech();
	afx_msg void OnEnKillfocusEditnoutputch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int m_nSamplingFreq;
	int m_nStartFreq;
	int m_nStopFreq;
};
