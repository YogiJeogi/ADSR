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
	// ��Ʈ�� ����
	CListCtrl m_CtrlListDesiredSignals;
	CListCtrl m_CtrlListRespSignals;
	double m_dbDesiredFrom;
	double m_dbDesiredTo;

	// ���� ���� ����
	CArray<int, int>	m_anMatchedDesiredSignals;
	CArray<int, int>	m_anSignDesiredSignals;
	// �� �迭�� �� ����ä�ο� ��ǥ��ȣ�� ������������ ���� Bool �迭�̴�. �Ǵ����� ��.
	CArray<bool,bool>   m_aryIsDesiredSignalExistInReationWithSubResponseChannel;
	// �� ������ �� ���� ä�ο� ��� ��ǥ��ȣ�� ��Ī�Ǿ����� ��Ÿ���� ����.
	int                 m_nSubMatchedDesiredSignals;

	// ���� ���� ����ü
	_FileSystem			m_sDesiredSignalFileInformation;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonFileSel();
	afx_msg void OnBnClickedButtonInsertDsign();
	afx_msg void OnBnClickedButtonDsignSign();
	afx_msg void OnBnClickedOk();
};
