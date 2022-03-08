#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDSignalsImport dialog

class CDSignalsImport : public CDialog
{
	DECLARE_DYNAMIC(CDSignalsImport)

public:
	CDSignalsImport(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDSignalsImport();

// Dialog Data
	enum { IDD = IDD_DIALOG_DSign_IMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	// 콘트롤 변수
	CListCtrl m_CtrlListDesiredSignals;
	CListCtrl m_CtrlListRespSignals;
	double m_dbDesiredFrom;
	double m_dbDesiredTo;

	// 상태 저장 변수
	CArray<int, int>	m_anMatchedDesiredSignals;
	CArray<int, int>	m_anSignDesiredSignals;
	// 이 배열은 부 응답채널에 목표신호가 더해졌는지에 대한 Bool 배열이다. 판단위한 것.
	CArray<bool,bool>   m_aryIsDesiredSignalExistInReationWithSubResponseChannel;
	// 이 변수는 섭 응답 채널에 몇개의 목표신호가 매칭되었나를 나타내는 변수.
	int                 m_nSubMatchedDesiredSignals;

	// 파일 정보 구조체
	_FileSystem			m_sDesiredSignalFileInformation;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFileSel();
	afx_msg void OnBnClickedButtonInsertDsign();
	afx_msg void OnBnClickedButtonDsignSign();
	afx_msg void OnBnClickedOk();
};
