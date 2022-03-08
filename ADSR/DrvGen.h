#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDrvGen dialog

class CDrvGen : public CDialog
{
	DECLARE_DYNAMIC(CDrvGen)

public:
	CDrvGen(bool IsFirstDrive=FALSE,CWnd* pParent = NULL);
	virtual ~CDrvGen();

// Dialog Data
	enum { IDD = IDD_DIALOG_UpDate_Drive };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool    m_bIsFirstDrive;
	VecDoub m_vecFirstDriveLevel;
	VecDoub m_vecErrorLevel;
	VecDoub m_vecOutWeightLevel;

public:

	CListCtrl m_ListCtrlErrorW;
	CComboBox m_ComCtrlRespElement;
	int m_nComboRespElement;
	int m_nEditRespValue;

	virtual BOOL OnInitDialog();


	afx_msg void OnBnClickedButtonRespApply();
	afx_msg void OnBnClickedOk();
};
