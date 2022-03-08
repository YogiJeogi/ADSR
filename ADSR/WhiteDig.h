#pragma once


// CWhiteDig dialog

class CWhiteDig : public CDialog
{
	DECLARE_DYNAMIC(CWhiteDig)

public:
	CWhiteDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWhiteDig();

// Dialog Data
	enum { IDD = IDD_DIALOG_White };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	static bool   m_bIsWhiteNoiseGenerated;

	int m_nStartFreq;
	int m_nBorderFreq;
	int m_nStopFreq;
	double m_dbSTD;
	double m_dbAlpha;
	double m_dbTime;
};
