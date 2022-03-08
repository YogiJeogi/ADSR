
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#define WM_HD_TF WM_USER+1
#define WM_HD_RTV WM_USER+2
#define WM_HD_UP WM_USER+3
#include "..\ADSRAlgorithms\typedef.h"
#include "ResultDoc.h"
#include "RealVDoc.h"


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
// �� ����
private:
	// ���ϵ� �������� ��ť��Ʈ�� ������������ �ڵ� 

	// ���ø��� ã������ �����̴�.

	CString				strDocName;

	// ���ø� 2���̸�
	// PreCreateWindow���� �ڵ��� �̸� ���Ѵ�.
	CDocTemplate *		pResultVTemplate;
	CDocTemplate *		pRealVTemplate;
	// ���ø��� ���صξ������� ���� Flag
	bool				m_fIsResultTemplate;
	bool                m_fIsRealTemplate;


	// â�� ����� ���� �ӽ��� �����̴�.
	CDocument *			pDocument;
	CFrameWnd *			pFrame;
	CMDIChildWnd*		pActiveChild;

	CResultVDoc *		pResultVDoc;
	CRealVDoc *			pRealVDoc;
	
public:
	// RealView�� ����Ÿ.
	Mat3DDoub			m_HAmplitude;
	Mat3DDoub			m_HAngle;

public:
	// RealVDlg���� ������ ä�� �ε����� ���⿡ �ӽ� �����Ѵ�.
	// ��ť��Ʈ�� m_CtrlRViewID or m_CtrlEViewID�ε����� ����
	// �� ID�� �����ȴ�.
	CArray<int, int>	m_aryEListSel;
	CArray<int, int>	m_aryRListSel;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource
						, DWORD dwDefaultStyle = WS_MAXIMIZE | WS_OVERLAPPEDWINDOW 
						, CWnd* pParentWnd = NULL
						, CCreateContext* pContext = NULL);

protected:  
	// ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnRealVCalled(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnResultViewCalled(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnUpDateDoc(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
public:
	/* ====================================================================
	 *    DAQ
	 * ==================================================================*/
	void DAQSIInitialization(void);
	void DAQTSInitialization(void);
	void OnEventSI(CNiDAQmxAsyncOperation asyncHandle, void* userData);
	void OnEventTS(CNiDAQmxAsyncOperation asyncHandle, void* userData);
	void RemoveEventHandler(void);
	void DAQStop(void);
	void DAQCleanUp(void);

	// �� �޸� ���� 
	void CleanUpIfNecessary(void);	

	std::auto_ptr<CNiDAQmxTask> m_readtask;
	std::auto_ptr<CNiDAQmxTask> m_writetask[3];
	std::auto_ptr<CNiDAQmxAnalogMultiChannelReader>  m_reader;
	std::auto_ptr<CNiDAQmxAnalogMultiChannelWriter>  m_writer[3];
	/////////////////////////////
	MatDoub			m_SIreaddata;
	MatDoub			m_TSreaddata;
	CNiReal64Matrix m_readdata;
	CNiReal64Matrix m_writedata[3];
	/////////////////////////////

	bool m_taskRunning;
	long m_samples;
	bool m_bIsFirstResp;



#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// DATA VIEWâ�� ���� ó�� �Լ���
	void CloseALLResultViewWindows();
	void CloseResultViewWindows();
	void CreatResultViewWindow(int TYPE, int nSelectedSignal, int nCheckedViewMode ,int UpdateWnd);
	void CloseRealViewWindows(void);
	void CreatRealViewWindows(int nE, int nR, int TypeOfGraph);

		// DATA VIEW�� ResultView�� ���� ó���Լ���
		void ResultViewType1DataLoading(void);
		void ResultViewType2DataLoading(void);
		void ResultViewType3DataLoading(void);
		afx_msg void OnMenuHelpSave();
};

