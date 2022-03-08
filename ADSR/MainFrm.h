
// MainFrm.h : CMainFrame 클래스의 인터페이스
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
// 내 정의
private:
	// 차일드 윈도우의 도큐멘트와 프레임윈도우 핸들 

	// 템플릿을 찾기위한 변수이다.

	CString				strDocName;

	// 템플릿 2개이며
	// PreCreateWindow에서 핸들을 미리 구한다.
	CDocTemplate *		pResultVTemplate;
	CDocTemplate *		pRealVTemplate;
	// 템플릿을 구해두었는지에 대한 Flag
	bool				m_fIsResultTemplate;
	bool                m_fIsRealTemplate;


	// 창을 만들기 위한 임시적 변수이다.
	CDocument *			pDocument;
	CFrameWnd *			pFrame;
	CMDIChildWnd*		pActiveChild;

	CResultVDoc *		pResultVDoc;
	CRealVDoc *			pRealVDoc;
	
public:
	// RealView용 데이타.
	Mat3DDoub			m_HAmplitude;
	Mat3DDoub			m_HAngle;

public:
	// RealVDlg에서 설정한 채널 인덱스를 여기에 임시 저장한다.
	// 도큐먼트의 m_CtrlRViewID or m_CtrlEViewID인덱스에 따라서
	// 뷰 ID가 결정된다.
	CArray<int, int>	m_aryEListSel;
	CArray<int, int>	m_aryRListSel;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource
						, DWORD dwDefaultStyle = WS_MAXIMIZE | WS_OVERLAPPEDWINDOW 
						, CWnd* pParentWnd = NULL
						, CCreateContext* pContext = NULL);

protected:  
	// 컨트롤 모음이 포함된 멤버입니다.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 생성된 메시지 맵 함수
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

	// 힙 메모리 삭제 
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

	// DATA VIEW창에 대한 처리 함수들
	void CloseALLResultViewWindows();
	void CloseResultViewWindows();
	void CreatResultViewWindow(int TYPE, int nSelectedSignal, int nCheckedViewMode ,int UpdateWnd);
	void CloseRealViewWindows(void);
	void CreatRealViewWindows(int nE, int nR, int TypeOfGraph);

		// DATA VIEW의 ResultView에 대한 처리함수들
		void ResultViewType1DataLoading(void);
		void ResultViewType2DataLoading(void);
		void ResultViewType3DataLoading(void);
		afx_msg void OnMenuHelpSave();
};

