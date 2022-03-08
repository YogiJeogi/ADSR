
// ADSR.h : ADSR 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.
#include "../ADSRAlgorithms/BaseHD.h"
#include "../ADSRAlgorithms/\SI_class.h"
#include "ResultVDig.h"
#include "StartDAQ.h"
#include <NiScalarVector.h>
#include "../ADSRAlgorithms/System.h"


/*=========================================
 * 구조체 선언
 *  이 구조체는 뷰 대화상자결과 데이타를 
 *  저장함으로써 결과 창과 소통하기 위한
 *  사용자 정의 구조체이다.
 *  ======================================*/
struct sResultViewUserData
{
	CArray<int,int> m_anSelectedCh1;
	CArray<int,int> m_anSelectedCh2;
	unsigned int	m_nNSelectedCh1;
	unsigned int	m_nNSelectedCh2;
};
struct sRealViewUserData
{

};
struct sRealResultViewUserData
{
	sResultViewUserData RTV;
	sRealViewUserData RV;
};
/* ============================================
 *	전역 맴버 SS가 선언되어 있다.
 *	
 *	모든 대화상자에 대한 선언이 되어있다.
 *	
 *	모든 메뉴 코맨드에 대한 선언이 되어있다.
 *
 * ==========================================*/

// 전역 맴버 선언
extern CSystem					System;
extern CCriticalSection			g_CriticalSection;
extern sRealResultViewUserData  g_sVUserData;


// CADSRApp:
// 이 클래스의 구현에 대해서는 ADSR.cpp을 참조하십시오.
//
 struct ProgressState 
 {
	 bool bSystemSet;
	 bool bExcChSet;
	 bool bRespChSet;
	 bool bWNGen;
	 bool bSIComplete;
	 bool bTargetImport;
	 bool bFirstDrive;
	 bool bUpDrive;
	 bool bTSComplete;
 } ;
 extern ProgressState ProgStates;

class CADSRApp : public CWinAppEx
{
public:
	CADSRApp();

private:


	// 시간데이타를 보기위한 패널이다.
	CResultVDig			* pResultVDlg;

	// SI를 위한 데이타 획득 및 진동 가진을 위한 패널이다.
	CStartDAQDlg		* pStartDlg;

// 재정의입니다.
public:
	virtual BOOL	InitInstance();

// 구현입니다.
/////////////////////////////////////////////////////

	MatDoub			m_SIWhiteNoiseTimeData;
	CNiReal64Matrix m_SIWhiteNoiseTimeDataNI;

	MatDoub			m_TSDriveTimeData;
	CNiReal64Matrix m_TSDriveTimeDataNI;

	void			Modalessdone();
	BOOL			m_bHiColorIcons;
	BOOL			m_bIsNewProject;
	BOOL			m_bIsSystemsetInitialized;

	int				m_numExciterChannels;
	int				m_numResponseChannels;
	CNiStringVector m_strvNIResponseChannel;
	CNiStringVector m_strvNIExciterChannel;

/////////////////////////////////////////////////////

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnAppAbout();
	afx_msg void OnMenuSysSysset();
	afx_msg void OnMenuSysExc();
	afx_msg void OnMenuSysResp();
	afx_msg void OnMenuSiWn();
	afx_msg void OnMenuSiSiv();
	afx_msg void OnUpdateMenuSysExc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSysResp(CCmdUI *pCmdUI);
	afx_msg void OnMenuSiWnv();
	afx_msg void OnMenuSiSi();
	afx_msg void OnMenuGraphCreat();
	afx_msg void OnMenuTsDsImport();
	afx_msg void OnMenuTsDrvgen();
	afx_msg void OnMenuTsDrvup();
	afx_msg void OnMenuTsDrivesys();
	afx_msg void OnResultSignals();
	afx_msg void OnResultHmodel();
	afx_msg void OnResultSignalcomparison();
	afx_msg void OnMenuSidata();
	afx_msg void OnTsResp();
	afx_msg void OnMenuTsdrive();
	afx_msg void OnUpdateMenuSiWnv(CCmdUI *pCmdUI);
	afx_msg void OnMenuSiresp();
	afx_msg void OnUpdateMenuSiWn(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuSiSi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDsImport(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrvgen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrvup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuTsDrivesys(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuGraphWin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateResultHmodel(CCmdUI *pCmdUI);
};

extern CADSRApp theApp;
